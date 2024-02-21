// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once
#include "MidiService.g.h"

namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiService : MidiServiceT<MidiService>
    {
        MidiService() = default;

        static midi2::MidiServicePingResponseSummary PingService(
            _In_ uint8_t const pingCount
            ) noexcept;

        static midi2::MidiServicePingResponseSummary PingService(
            _In_ uint8_t const pingCount,
            _In_ uint32_t const timeoutMilliseconds
            ) noexcept;

        static foundation::Collections::IVectorView<midi2::MidiServiceTransportPluginInformation> GetInstalledTransportPlugins();
        static foundation::Collections::IVectorView<midi2::MidiServiceMessageProcessingPluginInformation> GetInstalledMessageProcessingPlugins();

//        static uint32_t GetOutgoingMessageQueueMaxMessageCapacity() noexcept { return (uint32_t)MIDI_OUTGOING_MESSAGE_QUEUE_MAX_MESSAGE_COUNT; }

        static foundation::Collections::IVectorView<midi2::MidiServiceSessionInformation> GetActiveSessions() noexcept;

        static midi2::MidiServiceLoopbackEndpointCreationResult CreateTemporaryLoopbackEndpoints(
            _In_ winrt::guid const& associationId,
            _In_ midi2::MidiServiceLoopbackEndpointDefinition const& endpointDefinitionA,
            _In_ midi2::MidiServiceLoopbackEndpointDefinition const& endpointDefinitionB) noexcept;

        static bool RemoveTemporaryLoopbackEndpoints(_In_ winrt::guid const& associationId) noexcept;

        static midi2::MidiServiceConfigurationResponse UpdateTransportPluginConfiguration(
            _In_ midi2::IMidiServiceTransportPluginConfiguration const& configurationUpdate) noexcept;

        static midi2::MidiServiceConfigurationResponse UpdateProcessingPluginConfiguration(
            _In_ midi2::IMidiServiceMessageProcessingPluginConfiguration const& configurationUpdate) noexcept;




        static json::JsonObject InternalSendConfigurationJsonAndGetResponse(
            _In_ winrt::guid const& abstractionId, 
            _In_ json::JsonObject const& configObject) noexcept;

    private:


    };
}
namespace winrt::Windows::Devices::Midi2::factory_implementation
{
    struct MidiService : MidiServiceT<MidiService, implementation::MidiService, winrt::static_lifetime>
    {
    };
}
