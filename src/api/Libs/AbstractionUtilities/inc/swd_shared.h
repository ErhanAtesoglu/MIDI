// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================


#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Enumeration.h>


#pragma once

namespace Windows::Devices::Midi2::Internal
{

    std::wstring GetSwdStringProperty(_In_ std::wstring deviceInterfaceId, _In_ std::wstring propertyName, _In_ std::wstring defaultValue);

    std::wstring GetSwdPropertyVirtualEndpointAssociationId(_In_ std::wstring deviceInterfaceId);

    std::wstring GetSwdPropertyInstanceId(_In_ std::wstring deviceInterfaceId);
}