// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App SDK and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"
#include "MidiStreamMessageEndpointListener.h"
#include "MidiStreamMessageEndpointListener.g.cpp"


namespace winrt::Microsoft::Devices::Midi2::implementation
{
    void MidiStreamMessageEndpointListener::ProcessIncomingMessage(winrt::Windows::Devices::Midi2::IMidiInputConnection const& sourceConnection, winrt::Windows::Devices::Midi2::MidiMessageReceivedEventArgs const& args, bool& stopProcessing, bool& removeMessageFromQueue)
    {
        throw hresult_not_implemented();
    }
}
