// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"

namespace Windows::Devices::Midi2::Internal
{
    _Use_decl_annotations_
    DEVPROPERTY BuildEmptyDevProperty(DEVPROPKEY const key)
    {
        return DEVPROPERTY{ {key, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_EMPTY, 0, nullptr };

    }
}
