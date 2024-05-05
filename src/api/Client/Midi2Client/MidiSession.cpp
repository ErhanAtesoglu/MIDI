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

#include "MidiEndpointConnection.h"

//#include <atlcomcli.h>

namespace MIDI_CPP_NAMESPACE::implementation
{

    _Use_decl_annotations_
    midi2::MidiSession MidiSession::CreateSession(
        winrt::hstring const& sessionName
        ) noexcept
    {
        internal::LogInfo(__FUNCTION__, L"Session create");

        try
        {
            auto session = winrt::make_self<implementation::MidiSession>();

            session->SetName(sessionName);

            if (session->InternalStart())
            {
                return *session;
            }
            else
            {
                // this can happen is service is not available or running

                internal::LogInfo(__FUNCTION__, L"Session start failed. Returning nullptr");

                return nullptr;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L"hresult exception initializing creating session. Service may be unavailable.", ex);

            return nullptr;
        }
        catch (...)
        {
            internal::LogGeneralError(__FUNCTION__, L"Exception initializing creating session. Service may be unavailable.");

            return nullptr;
        }
    }

    // Internal method called inside the API to connect to the abstraction. Called by the code which creates
    // the session instance
    _Use_decl_annotations_
    bool MidiSession::InternalStart()
    {
        internal::LogInfo(__FUNCTION__, L"Start Session ");

        try
        {
            // We're talking to the service, so use the MIDI Service abstraction, not a KS or other one
            m_serviceAbstraction = winrt::create_instance<IMidiAbstraction>(__uuidof(Midi2MidiSrvAbstraction), CLSCTX_ALL);

            if (m_serviceAbstraction != nullptr)
            {
                // the session id is created on the client and provided in calls to the endpoints
                m_id = foundation::GuidHelper::CreateNewGuid();

                m_isOpen = true;


                // register the session

                if (SUCCEEDED(m_serviceAbstraction->Activate(__uuidof(IMidiSessionTracker), (void**)&m_sessionTracker)))
                {
                    auto initHR = m_sessionTracker->Initialize();
                    if (FAILED(initHR))
                    {
                        internal::LogHresultError(__FUNCTION__, L"Unable to initialize session tracker", initHR);
                        return false;
                    }

                    auto addHR = m_sessionTracker->AddClientSession(m_id, m_name.c_str());
                    if (FAILED(addHR))
                    {
                        internal::LogHresultError(__FUNCTION__, L"Unable to add client session to session tracker", addHR);
                        return false;
                    }
                }
                else
                {
                    internal::LogGeneralError(__FUNCTION__, L"Error activating IMidiSessionTracker.");

                    return false;
                }
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Error starting session. Service abstraction is nullptr.");

                return false;
            }

        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L"hresult exception starting session. Service may be unavailable.", ex);

            return false;
        }
        catch (...)
        {
            internal::LogGeneralError(__FUNCTION__, L"Exception starting session.");

            return false;
        }

        return true;
    }

    _Use_decl_annotations_
    bool MidiSession::UpdateName(winrt::hstring const& newName) noexcept
    {
        internal::LogInfo(__FUNCTION__, L"Enter");

        auto cleanName = internal::TrimmedHStringCopy(newName);

        // this can be called only if we've already initialized the session tracker
        if (m_sessionTracker)
        {
            DWORD clientProcessId = GetCurrentProcessId();

            auto hr = m_sessionTracker->UpdateClientSessionName(m_id, cleanName.c_str(), clientProcessId);

            if (SUCCEEDED(hr))
            {
                m_name = cleanName;
                return true;
            }
            else
            {
                internal::LogHresultError(__FUNCTION__, L"Unable to update session name", hr);

                return false;
            }
        }
        else
        {
            internal::LogGeneralError(__FUNCTION__, L"Session tracker interface wasn't already initialized.");

            return false;
        }
    }




    void MidiSession::Close() noexcept
    {
        internal::LogInfo(__FUNCTION__, L"Closing session");

        if (!m_isOpen) return;

        try
        {
            if (m_serviceAbstraction != nullptr)
            {
                // TODO: Call any cleanup method on the service
                for (auto connection : m_connections)
                {
                    // close the one connection
                    //connection.Value().as<foundation::IClosable>().Close();

                    auto c = winrt::get_self<implementation::MidiEndpointConnection>(connection.Value());
                    c->InternalClose();
                }

                m_serviceAbstraction = nullptr;
            }

            if (m_sessionTracker != nullptr)
            {
                m_sessionTracker->RemoveClientSession(m_id);

                m_sessionTracker = nullptr;
            }

            m_connections.Clear();

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
