// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once

#include "MidiInputEndpointConnection.g.h"
#include "MidiEndpointConnection.h"

#include "MidiMessageReceivedEventArgs.h"

#include "midi_service_interface.h"


namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiInputEndpointConnection : MidiInputEndpointConnectionT<
        MidiInputEndpointConnection, 
        Windows::Devices::Midi2::implementation::MidiEndpointConnection,
        IMidiCallback>
    {
        MidiInputEndpointConnection() = default;

        static hstring GetDeviceSelector() noexcept { return L"System.Devices.InterfaceClassGuid:=\"{AE174174-6396-4DEE-AC9E-1E9C6F403230}\" AND System.Devices.InterfaceEnabled:=System.StructuredQueryType.Boolean#True"; }

        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Devices::Midi2::IMidiEndpointMessageListener> MessageListeners() { return m_messageListeners; }



        STDMETHOD(Callback)(_In_ PVOID Data, _In_ UINT Size, _In_ LONGLONG Position) override;

        inline winrt::event_token MessageReceived(_In_ winrt::Windows::Foundation::TypedEventHandler<IInspectable, winrt::Windows::Devices::Midi2::MidiMessageReceivedEventArgs> const& handler)
        {
            return m_messagesReceivedEvent.add(handler);
        }

        inline void MessageReceived(_In_ winrt::event_token const& token) noexcept
        {
            if (m_messagesReceivedEvent) m_messagesReceivedEvent.remove(token);
        }


        _Success_(return == true)
        bool InternalInitialize();

        _Success_(return == true)
        bool Open();


    private:
        winrt::event<winrt::Windows::Foundation::TypedEventHandler<IInspectable, winrt::Windows::Devices::Midi2::MidiMessageReceivedEventArgs>> m_messagesReceivedEvent;

        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Devices::Midi2::IMidiEndpointMessageListener>
            m_messageListeners{ winrt::single_threaded_vector<winrt::Windows::Devices::Midi2::IMidiEndpointMessageListener>() };

    };
}
namespace winrt::Windows::Devices::Midi2::factory_implementation
{
    struct MidiInputEndpointConnection : MidiInputEndpointConnectionT<MidiInputEndpointConnection, implementation::MidiInputEndpointConnection>
    {
    };
}