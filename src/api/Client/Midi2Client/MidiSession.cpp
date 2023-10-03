// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"
#include "MidiSession.h"
#include "MidiSession.g.cpp"

#include "MidiInputEndpointConnection.h"
#include "MidiOutputEndpointConnection.h"
#include "MidiBidirectionalEndpointConnection.h"

#include "MidiEndpointInformationConfigurator.h"

#include "string_util.h"


namespace winrt::Windows::Devices::Midi2::implementation
{

    _Use_decl_annotations_
    midi2::MidiSession MidiSession::CreateSession(
        winrt::hstring const& sessionName,
        midi2::MidiSessionSettings const& settings
        ) noexcept
    {
        try
        {
            auto session = winrt::make_self<implementation::MidiSession>();

            session->SetName(sessionName);
            session->SetSettings(settings);

            if (session->InternalStart())
            {
                return *session;
            }
            else
            {
                // TODO: Returning nullptr on failure is not super useful per feedback. Consider throwing an hresult?

                return nullptr;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception initializing creating session. Service may be unavailable.", ex);

            // TODO: throwing an hresult exception here would be preferred vs returning null

            return nullptr;
        }
       
    }

    _Use_decl_annotations_
    midi2::MidiSession MidiSession::CreateSession(
        winrt::hstring const& sessionName
        ) noexcept
    {
        return CreateSession(sessionName, MidiSessionSettings());
    }

    // Internal method called inside the API to connect to the abstraction. Called by the code which creates
    // the session instance
    bool MidiSession::InternalStart()
    {
        try
        {
            // We're talking to the service, so use the MIDI Service abstraction, not a KS or other one
            m_serviceAbstraction = winrt::create_instance<IMidiAbstraction>(__uuidof(Midi2MidiSrvAbstraction), CLSCTX_ALL);

            // TODO: Not sure if service will need to provide the Id, or we can simply gen a GUID and send it up
            // that's why the assignment is in this function and not in CreateSession()
            m_id = foundation::GuidHelper::CreateNewGuid();

            m_isOpen = true;

            // create the virtual device manager

            auto virtualDeviceManager = winrt::make_self<implementation::MidiVirtualDeviceManager>();
            if (virtualDeviceManager != nullptr)
            {
                virtualDeviceManager->Initialize(m_serviceAbstraction);

                m_virtualDeviceManager = *virtualDeviceManager;
            }

        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception starting session. Service may be unavailable.", ex);

            return false;
        }

        return true;
    }

    _Use_decl_annotations_
    winrt::hstring MidiSession::NormalizeDeviceId(const winrt::hstring& endpointDeviceId)
    {
        if (endpointDeviceId.empty()) return endpointDeviceId;

        return internal::ToUpperTrimmedHStringCopy(endpointDeviceId);
    }

    // TODO: The connection open code is almost identical across the four types. Refactor with an internal template function or something.

    // Bidirectional ===========================================================================================================

    _Use_decl_annotations_
    midi2::MidiBidirectionalEndpointConnection MidiSession::ConnectBidirectionalEndpoint(
        winrt::hstring const& endpointDeviceId,
        midi2::MidiBidirectionalEndpointOpenOptions const& options,
        midi2::IMidiEndpointDefinedConnectionSettings const& /*settings*/
        ) noexcept
    {
        try
        {
            auto normalizedDeviceId = NormalizeDeviceId(endpointDeviceId);
            auto endpointConnection = winrt::make_self<implementation::MidiBidirectionalEndpointConnection>();

            // generate internal endpoint Id
            auto connectionInstanceId = foundation::GuidHelper::CreateNewGuid();

            if (endpointConnection->InternalInitialize(m_serviceAbstraction, connectionInstanceId, normalizedDeviceId, options))
            {
                m_connections.Insert(connectionInstanceId, (const midi2::IMidiEndpointConnection)(*endpointConnection));

                // TODO: This value needs to come from configuration for this endpoint. It comes from
                // the user settings property store, and then the open options
                bool autoHandleEndpointMessages = true;

                if (autoHandleEndpointMessages)
                {
                    //auto endpointInformationListener = winrt::make_self<implementation::MidiEndpointInformationEndpointListener>();

                    //endpointInformationListener->Id(L"auto_EndpointInformationListener");
                    //endpointInformationListener->Name(L"Automatic Endpoint Information Message Handler");
                    //endpointInformationListener->InputConnection(endpointConnection.as<IMidiInputConnection>());
                    //endpointInformationListener->Initialize();
                    //                   
                    //endpointConnection->MessageListeners().Append(*endpointInformationListener);
                }

                return *endpointConnection;
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"WinRT Endpoint connection wouldn't start");

                // TODO: Cleanup

                return nullptr;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception connecting to endpoint. Service or endpoint may be unavailable, or endpoint may not be the correct type.", ex);

            return nullptr;
        }
    }

    _Use_decl_annotations_
    midi2::MidiBidirectionalEndpointConnection MidiSession::ConnectBidirectionalEndpoint(
        winrt::hstring const& endpointDeviceId,
        midi2::MidiBidirectionalEndpointOpenOptions const& options
        ) noexcept
    {
        return ConnectBidirectionalEndpoint(endpointDeviceId, options, nullptr);
    }

    _Use_decl_annotations_
    midi2::MidiBidirectionalEndpointConnection MidiSession::ConnectBidirectionalEndpoint(
        winrt::hstring const& endpointDeviceId
        ) noexcept
    {
        return ConnectBidirectionalEndpoint(endpointDeviceId, nullptr, nullptr);
    }


    // Bidirectional Aggregated ===========================================================================================================

    _Use_decl_annotations_
    midi2::MidiBidirectionalAggregatedEndpointConnection MidiSession::ConnectBidirectionalAggregatedEndpoint(
        winrt::hstring const& inputEndpointDeviceId,
        winrt::hstring const& outputEndpointDeviceId ,
        midi2::MidiBidirectionalAggregatedEndpointOpenOptions const& /*options*/,
        midi2::IMidiEndpointDefinedConnectionSettings const& /*settings*/
        ) noexcept
    {
        try
        {
            auto normalizedInputDeviceId = NormalizeDeviceId(inputEndpointDeviceId);
            auto normalizedOutputDeviceId = NormalizeDeviceId(outputEndpointDeviceId);
            auto endpointConnection = winrt::make_self<implementation::MidiBidirectionalAggregatedEndpointConnection>();

            // generate internal endpoint Id
            auto connectionInstanceId = foundation::GuidHelper::CreateNewGuid();


            if (endpointConnection->InternalInitialize(m_serviceAbstraction, connectionInstanceId, normalizedInputDeviceId, normalizedOutputDeviceId))
            {
                m_connections.Insert(connectionInstanceId, (const midi2::IMidiEndpointConnection)(*endpointConnection));

                // TODO: This value needs to come from configuration for this endpoint. It comes from
                // the user settings property store, and then the open options
                bool autoHandleEndpointMessages = true;

                if (autoHandleEndpointMessages)
                {
                    //auto endpointInformationListener = winrt::make_self<implementation::MidiEndpointInformationEndpointListener>();

                    //endpointInformationListener->Id(L"auto_EndpointInformationListener");
                    //endpointInformationListener->Name(L"Automatic Endpoint Information Message Handler");
                    //endpointInformationListener->InputConnection(endpointConnection.as<IMidiInputConnection>());
                    //endpointInformationListener->Initialize();
                    //                   
                    //endpointConnection->MessageListeners().Append(*endpointInformationListener);
                }

                return *endpointConnection;
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"WinRT Endpoint connection wouldn't start");

                // TODO: Cleanup

                return nullptr;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception connecting to endpoint. Service or endpoint may be unavailable, or endpoint may not be the correct type.", ex);

            return nullptr;
        }
    }

    _Use_decl_annotations_
    midi2::MidiBidirectionalAggregatedEndpointConnection MidiSession::ConnectBidirectionalAggregatedEndpoint(
        winrt::hstring const& inputEndpointDeviceId,
        winrt::hstring const& outputEndpointDeviceId,
        midi2::MidiBidirectionalAggregatedEndpointOpenOptions const& options
        ) noexcept
    {
        return ConnectBidirectionalAggregatedEndpoint(inputEndpointDeviceId, outputEndpointDeviceId, options, nullptr);
    }

    _Use_decl_annotations_
    midi2::MidiBidirectionalAggregatedEndpointConnection MidiSession::ConnectBidirectionalAggregatedEndpoint(
        winrt::hstring const& inputEndpointDeviceId,
        winrt::hstring const& outputEndpointDeviceId
        ) noexcept
    {
        return ConnectBidirectionalAggregatedEndpoint(inputEndpointDeviceId, outputEndpointDeviceId, nullptr, nullptr);
    }









    // Output ===================================================================================================================

    _Use_decl_annotations_
    midi2::MidiOutputEndpointConnection MidiSession::ConnectOutputEndpoint(
        winrt::hstring const& deviceId,
        midi2::MidiOutputEndpointOpenOptions const& /*options*/,
        midi2::IMidiEndpointDefinedConnectionSettings const& /*settings*/
        ) noexcept
    {
        try
        {
            auto normalizedDeviceId = NormalizeDeviceId(deviceId);
            auto endpointConnection = winrt::make_self<implementation::MidiOutputEndpointConnection>();

            // generate internal endpoint Id
            auto connectionInstanceId = foundation::GuidHelper::CreateNewGuid();

            if (endpointConnection->InternalInitialize(m_serviceAbstraction, connectionInstanceId, normalizedDeviceId))
            {
                m_connections.Insert(connectionInstanceId, (const midi2::IMidiEndpointConnection)(*endpointConnection));

                return *endpointConnection;
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"WinRT Endpoint connection wouldn't start");

                // TODO: Cleanup

                return nullptr;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception connecting to endpoint. Service or endpoint may be unavailable, or endpoint may not be the correct type.", ex);

            return nullptr;
        }



    }


    _Use_decl_annotations_
    midi2::MidiOutputEndpointConnection MidiSession::ConnectOutputEndpoint(
        winrt::hstring const& deviceId,
        midi2::MidiOutputEndpointOpenOptions const& options
        ) noexcept
    {
        return ConnectOutputEndpoint(deviceId, options, nullptr);
    }

    _Use_decl_annotations_
    midi2::MidiOutputEndpointConnection MidiSession::ConnectOutputEndpoint(
        winrt::hstring const& deviceId
        ) noexcept
    {
        return ConnectOutputEndpoint(deviceId, nullptr, nullptr);
    }



    // Input ==================================================================================================================

    _Use_decl_annotations_
    midi2::MidiInputEndpointConnection MidiSession::ConnectInputEndpoint(
        winrt::hstring const& deviceId,
        midi2::MidiInputEndpointOpenOptions const& /* options */,
        midi2::IMidiEndpointDefinedConnectionSettings const& /*settings*/
        ) noexcept
    {
        try
        {
            auto normalizedDeviceId = NormalizeDeviceId(deviceId);
            auto endpointConnection = winrt::make_self<implementation::MidiInputEndpointConnection>();

            // generate internal endpoint Id
            auto connectionInstanceId = foundation::GuidHelper::CreateNewGuid();

            if (endpointConnection->InternalInitialize(m_serviceAbstraction, connectionInstanceId, normalizedDeviceId))
            {
                m_connections.Insert(connectionInstanceId, (const midi2::IMidiEndpointConnection)(*endpointConnection));

                // TODO: This value needs to come from configuration for this endpoint. It comes from
                // the user settings property store, and then the open options
                bool autoHandleEndpointMessages = true;

                if (autoHandleEndpointMessages)
                {
                    //auto endpointInformationListener = winrt::make_self<implementation::MidiEndpointInformationEndpointListener>();

                    //endpointInformationListener->Id(L"auto_EndpointInformationListener");
                    //endpointInformationListener->Name(L"Automatic Endpoint Information Message Handler");
                    //endpointInformationListener->InputConnection(endpointConnection.as<IMidiInputConnection>());
                    //endpointInformationListener->Initialize();
                    //                   
                    //endpointConnection->MessageListeners().Append(*endpointInformationListener);
                }

                return *endpointConnection;
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"WinRT Endpoint connection wouldn't start");

                // TODO: Cleanup

                return nullptr;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception connecting to endpoint. Service or endpoint may be unavailable, or endpoint may not be the correct type.", ex);

            return nullptr;
        }
    }

    _Use_decl_annotations_
    midi2::MidiInputEndpointConnection MidiSession::ConnectInputEndpoint(
        winrt::hstring const& deviceId,
        midi2::MidiInputEndpointOpenOptions const& options
        ) noexcept
    {
        return ConnectInputEndpoint(deviceId, options, nullptr);
    }

    _Use_decl_annotations_
    midi2::MidiInputEndpointConnection MidiSession::ConnectInputEndpoint(
        winrt::hstring const& deviceId
        ) noexcept
    {
        return ConnectInputEndpoint(deviceId, nullptr, nullptr);
    }




    _Use_decl_annotations_
    void MidiSession::DisconnectEndpointConnection(
        winrt::guid const& endpointConnectionId
        ) noexcept
    {
        try
        {
            if (m_connections.HasKey(endpointConnectionId))
            {
                // todo: disconnect the endpoint from the service, call Close() etc.

                m_connections.Lookup(endpointConnectionId).as<foundation::IClosable>().Close();

                m_connections.Remove(endpointConnectionId);
            }
            else
            {
                // endpoint already disconnected. No need to except or anything, just exit.
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception disconnecting from endpoint.", ex);

            return;
        }
        catch (...)
        {
            internal::LogGeneralError(__FUNCTION__, L" Unknown exception disconnecting endpoint connection");
        }
    }



    void MidiSession::Close() noexcept
    {
        if (!m_isOpen) return;

        try
        {
            m_connections.Clear();

            if (m_serviceAbstraction != nullptr)
            {
                // TODO: Call any cleanup method on the service

                m_serviceAbstraction = nullptr;
            }

            // Id is no longer valid, and session is not open. Clear these in case the client tries to use the held reference
            //m_id.clear();
            m_isOpen = false;
            m_mmcssTaskId = 0;

        }
        catch (...)
        {
            internal::LogGeneralError(__FUNCTION__, L"Exception on close");
        }

    }


    MidiSession::~MidiSession() noexcept
    {
        if (m_isOpen)
        {
            Close();
        }
    }



}
