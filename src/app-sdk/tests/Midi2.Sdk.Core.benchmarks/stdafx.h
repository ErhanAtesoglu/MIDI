// Copyright (c) Microsoft Corporation. All rights reserved.
#pragma once


#ifndef STDAFX_H
#define STDAFX_H


#include <windows.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Enumeration.h>

#include <winrt/Microsoft.Devices.Midi2.h>
#include <winrt/Microsoft.Devices.Midi2.Diagnostics.h>

using namespace winrt::Microsoft::Devices::Midi2;
using namespace winrt::Microsoft::Devices::Midi2::Diagnostics;

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <functional>


#include <avrt.h>
#include <wil\resource.h>
#include <wil\result_macros.h>
#include <ppltasks.h>
#include <WexTestClass.h>
#include <Objbase.h>
#include "loopback_ids.h"
#include "MidiDefs.h"


#include "MidiBenchmarks.h"
#include "MidiSchedulerBenchmarks.h"

#ifndef LOG_OUTPUT
#define LOG_OUTPUT(fmt, ...)  WEX::Logging::Log::Comment(WEX::Common::String().Format(fmt, __VA_ARGS__))
#endif



#endif