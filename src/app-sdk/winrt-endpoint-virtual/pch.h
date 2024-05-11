﻿// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================


#pragma once

//#define _VSDESIGNER_DONT_LOAD_AS_DLL


#include <unknwn.h>

#include <Windows.h>

//#include <wil/cppwinrt.h> // must be before the first C++ WinRT header
//#include <wil/result.h>

#include <wil\resource.h>
#include <wil\result_macros.h>
#include <wil\tracelogging.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Devices.Midi.h>

#include <winrt/Windows.Data.Json.h>
namespace json = ::winrt::Windows::Data::Json;
namespace enumeration = ::winrt::Windows::Devices::Enumeration;
namespace midi1 = ::winrt::Windows::Devices::Midi;

#include <winrt/Microsoft.Devices.Midi2.h>
namespace midi2 = ::winrt::Microsoft::Devices::Midi2;


#include <stdint.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <cwctype>
#include <queue>
#include <mutex>
#include <format>
#include <filesystem>

// include this before any project includes
#include "namespace_defines.h"

// internal
#include "trace_logging.h"

#include "ump_helpers.h"
#include "memory_buffer.h"
#include "wstring_util.h"
#include "midi_group_terminal_blocks.h"

// AbstractionUtilities

// shared
#include "midi_ump.h"   // general shared
#include "loopback_ids.h"
#include <midi_timestamp.h>

//#include <wil/resource.h>

namespace foundation = ::winrt::Windows::Foundation;
namespace collections = ::winrt::Windows::Foundation::Collections;

#include "hstring_util.h"
#include "wstring_util.h"
namespace internal = ::WindowsMidiServicesInternal;

#include "MidiDefs.h"
#include "WindowsMidiServices.h"
#include "WindowsMidiServices_i.c"
#include "Midi2MidiSrvAbstraction.h"

#include "json_defs.h"
#include "json_helpers.h"
#include "swd_helpers.h"
#include "resource_util.h"
#include "ping_ump_types.h"

//#include "MidiXProc.h"
#include "resource.h"

namespace winrt::Microsoft::Devices::Midi2::Endpoints::Virtual {};
namespace winrt::Microsoft::Devices::Midi2::Endpoints::Virtual::implementation {};

namespace virt = ::winrt::Microsoft::Devices::Midi2::Endpoints::Virtual;
namespace implementation = ::MIDI_CPP_NAMESPACE::implementation;

#include "midi_stream_message_defs.h"
#include "midi_ump_message_defs.h"

#include <Devpropdef.h>
#include "MidiDefs.h"
#include "midi_function_block_prop_util.h"



#include "MidiStreamConfigurationRequestedSettings.h"
#include "MidiStreamConfigurationRequestReceivedEventArgs.h"
#include "MidiVirtualEndpointDevice.h"
#include "MidiVirtualEndpointDeviceDefinition.h"

#include "MidiSession.h"
