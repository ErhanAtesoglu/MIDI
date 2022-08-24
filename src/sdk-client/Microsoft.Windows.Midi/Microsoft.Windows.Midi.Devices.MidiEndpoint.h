#pragma once
#include "Microsoft.Windows.Midi.Devices.MidiEndpoint.g.h"

// WARNING: This file is automatically generated by a tool. Do not directly
// add this file to your project, as any changes you make will be lost.
// This file is a stub you can use as a starting point for your implementation.
//
// To add a copy of this file to your project:
//   1. Copy this file from its original location to the location where you store 
//      your other source files (e.g. the project root). 
//   2. Add the copied file to your project. In Visual Studio, you can use 
//      Project -> Add Existing Item.
//   3. Delete this comment and the 'static_assert' (below) from the copied file.
//      Do not modify the original file.
//
// To update an existing file in your project:
//   1. Copy the relevant changes from this file and merge them into the copy 
//      you made previously.
//    
// This assertion helps prevent accidental modification of generated files.
////static_assert(false, "This file is generated by a tool and will be overwritten. Open this error and view the comment for assistance.");

namespace winrt::Microsoft::Windows::Midi::Devices::implementation
{
    struct MidiEndpoint : MidiEndpointT<MidiEndpoint>
    {
        MidiEndpoint() = default;

        static winrt::Microsoft::Windows::Midi::Devices::MidiEndpoint Open(winrt::Microsoft::Windows::Midi::Enumeration::MidiEndpointInformation const& information);
        static winrt::Microsoft::Windows::Midi::Devices::MidiEndpoint Open(winrt::guid const& deviceId, winrt::guid const& endpointId);
        winrt::Microsoft::Windows::Midi::Enumeration::MidiEndpointInformation Information();
        winrt::event_token MessageReceived(winrt::Windows::Foundation::EventHandler<winrt::Microsoft::Windows::Midi::MidiMessageReceivedEventArgs> const& handler);
        void MessageReceived(winrt::event_token const& token) noexcept;
        void Close();
        void StartSendingJRClock(uint8_t group, uint8_t channel);
        void StopSendingJRClock(uint8_t group, uint8_t channel);
        void SendUmp(winrt::Microsoft::Windows::Midi::Messages::Ump const& message);
        void SendUmp(uint32_t word0);
        void SendUmp(uint32_t word0, uint32_t word1);
        void SendUmp(uint32_t word0, uint32_t word1, uint32_t word2);
        void SendUmp(uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3);
        void SendUmpWithJRTimestamp(winrt::Microsoft::Windows::Midi::Messages::Ump const& message);
        void SendUmpWithJRTimestamp(uint32_t word0);
        void SendUmpWithJRTimestamp(uint32_t word0, uint32_t word1);
        void SendUmpWithJRTimestamp(uint32_t word0, uint32_t word1, uint32_t word2);
        void SendUmpWithJRTimestamp(uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3);
    };
}
namespace winrt::Microsoft::Windows::Midi::Devices::factory_implementation
{
    struct MidiEndpoint : MidiEndpointT<MidiEndpoint, implementation::MidiEndpoint>
    {
    };
}