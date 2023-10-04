// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================



#pragma once

#include <pch.h>

#include "MidiBidirectionalAggregatedEndpointConnection.g.h"

#include "midi_service_interface.h"

#include "MidiMessageReceivedEventArgs.h"

#include "MidiUmp32.h"
#include "MidiUmp64.h"
#include "MidiUmp96.h"
#include "MidiUmp128.h"

#include "InternalMidiConnectionCommon.h"
#include "InternalMidiInputConnection.h"
#include "InternalMidiOutputConnection.h"
#include "InternalMidiFunctionBlockDevice.h"
#include "InternalMidiEndpointInformationDevice.h"

namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiBidirectionalAggregatedEndpointConnection : 
        MidiBidirectionalAggregatedEndpointConnectionT<
            MidiBidirectionalAggregatedEndpointConnection, 
            IMidiCallback>,
        public internal::InternalMidiConnectionCommon,
        public internal::InternalMidiInputConnection<IMidiIn>,
        public internal::InternalMidiOutputConnection<IMidiOut>,
        public internal::InternalMidiFunctionBlockDevice,
        public internal::InternalMidiEndpointInformationDevice

    {
        MidiBidirectionalAggregatedEndpointConnection() = default;
        ~MidiBidirectionalAggregatedEndpointConnection();

        STDMETHOD(Callback)(_In_ PVOID data, _In_ UINT size, _In_ LONGLONG position) override
        {
            return CallbackImpl(*this, data, size, position);
        }


        _Success_(return == true)
        bool InternalInitialize(
            _In_ winrt::com_ptr<IMidiAbstraction> serviceAbstraction,
            _In_ winrt::guid const connectionId,
            _In_ winrt::hstring const inputEndpointDeviceId,
            _In_ winrt::hstring const outputEndpointDeviceId
        );

        _Success_(return == true)
        bool Open();

        // IClosable
        void Close();

    private:
        bool m_closeHasBeenCalled{ false };

    };
}