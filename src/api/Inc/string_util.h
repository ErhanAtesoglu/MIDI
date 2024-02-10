
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once

#include <string>
#include <cwctype>

namespace Windows::Devices::Midi2::Internal
{
    

    inline void InPlaceToUpper(_Inout_ std::wstring &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), towupper);
    }

    inline void InPlaceToLower(_Inout_ std::wstring &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), towlower);
    }

    inline void InPlaceTrim(_Inout_ std::wstring& ws)
    {
        std::wstring whitespace = L" \0\t\n\r";

        size_t index = ws.find_first_not_of(whitespace);
        if (index != std::wstring::npos)
        {
            ws.erase(0, index);
        }

        index = ws.find_last_not_of(whitespace);
        ws.resize(index + 1);
    }

    inline std::wstring TrimmedWStringCopy(_In_ std::wstring ws)
    {
        std::wstring newString{ ws };

        InPlaceTrim(newString);

        return newString;
    }

    inline std::wstring ToUpperWStringCopy(_In_ std::wstring s)
    {
        std::wstring ws{ s };
        InPlaceToUpper(ws);

        return ws;
    }

    inline std::wstring ToLowerWStringCopy(_In_ std::wstring s)
    {
        std::wstring ws{ s };
        InPlaceToLower(ws);

        return ws;
    }


    inline std::wstring ToUpperTrimmedWStringCopy(_In_ std::wstring s)
    {
        return ToUpperWStringCopy(TrimmedWStringCopy(s));
    }

    inline std::wstring ToLowerTrimmedWStringCopy(_In_ std::wstring s)
    {
        return ToLowerWStringCopy(TrimmedWStringCopy(s));
    }

    // TODO: this could use substr and take one op instad of two
    inline winrt::hstring TrimmedHStringCopy(_In_ std::wstring ws)
    {
        return winrt::hstring{ TrimmedWStringCopy(ws) };
    }

    inline winrt::hstring TrimmedHStringCopy(_In_ winrt::hstring s)
    {
        std::wstring ws{ s };

        return TrimmedHStringCopy(ws);
    }

    inline winrt::hstring ToUpperHStringCopy(_In_ winrt::hstring s)
    {
        std::wstring ws{ s };
        std::transform(ws.begin(), ws.end(), ws.begin(), std::towupper); 

        return winrt::hstring{ ws };
    }

    inline winrt::hstring ToLowerHStringCopy(_In_ winrt::hstring s)
    {
        std::wstring ws{ s };
        std::transform(ws.begin(), ws.end(), ws.begin(), std::towlower);

        return winrt::hstring{ ws };
    }

    inline winrt::hstring ToUpperTrimmedHStringCopy(_In_ winrt::hstring s)
    {
        return ToUpperHStringCopy(TrimmedHStringCopy(s));
    }

    inline winrt::hstring ToLowerTrimmedHStringCopy(_In_ winrt::hstring s)
    {
        return ToLowerHStringCopy(TrimmedHStringCopy(s));
    }





    // This is just to convert all GUIDs to the same case. It does
    // not add or remove opening / closing brackets
    inline std::wstring NormalizeGuidStringCopy(_In_ std::wstring guidString)
    {
        return ToUpperTrimmedWStringCopy(guidString);
    }

    // This is for the device instance id. Not to be confused with the interface id
    inline std::wstring NormalizeDeviceInstanceIdWStringCopy(_In_ std::wstring deviceInstanceId)
    {
        return ToUpperTrimmedWStringCopy(deviceInstanceId);
    }
    // This is for the device instance id. Not to be confused with the interface id
    inline winrt::hstring NormalizeDeviceInstanceIdHStringCopy(_In_ winrt::hstring deviceInstanceId)
    {
        return ToUpperTrimmedHStringCopy(deviceInstanceId);
    }

    //// This is for the endpoint device interface id (the long SWD id with the GUID)
    //inline std::wstring NormalizeEndpointInterfaceIdCopy(_In_ std::wstring endpointInterfaceId)
    //{
    //    return ToLowerTrimmedWStringCopy(endpointInterfaceId);
    //}


    // This is for the endpoint device interface id (the long SWD id with the GUID)
    inline winrt::hstring NormalizeEndpointInterfaceIdHStringCopy(_In_ winrt::hstring endpointInterfaceId)
    {
        return ToLowerTrimmedHStringCopy(endpointInterfaceId);
    }

    // This is for the endpoint device interface id (the long SWD id with the GUID)
    inline std::wstring NormalizeEndpointInterfaceIdWStringCopy(_In_ std::wstring endpointInterfaceId)
    {
        return ToLowerTrimmedWStringCopy(endpointInterfaceId);
    }

    // used for searching for a substring in an endpoint interface id. Matches case with
    // what NormalizeEndpointInterfaceIdCopy produces
    inline bool EndpointInterfaceIdContainsString(_In_ std::wstring endpointInterfaceId, _In_ std::wstring searchFor)
    {
        auto id = NormalizeEndpointInterfaceIdWStringCopy(endpointInterfaceId);
        auto sub = ToLowerWStringCopy(searchFor);             // match case with NormalizeEndpointInterfaceIdCopy

        if (id == L"" || sub == L"")
        {
            return false;
        }

        return id.find(sub) != std::wstring::npos;
    }





    // note that this produces a GUID with uppercase letters and enclosing braces
    inline std::wstring GuidToString(_In_ GUID guid)
    {
        LPOLESTR str;
        if (SUCCEEDED(StringFromCLSID(guid, &str)))
        {
            std::wstring guidString{ str };

            ::CoTaskMemFree(str);

            return guidString;
        }
        else
        {
            return L"";
        }
    }

    inline GUID StringToGuid(_In_ std::wstring value)
    {
        // this fails when {} are included
//        winrt::guid resultingGuid = winrt::guid{ value };

        GUID g;

        if (SUCCEEDED(CLSIDFromString(value.c_str(), &g)))
        {
            return g;
        }
        else
        {
            // return the empty GUID. This is a bit dumb, honestly
            return g;
        }
        
    }



    inline std::wstring SystemTimeToDateTimeString(SYSTEMTIME const& time)
    {
        std::wstring dateBuffer;
        dateBuffer.reserve(50);

        std::wstring timeBuffer;
        timeBuffer.reserve(50);

        GetDateFormatEx(
            LOCALE_NAME_SYSTEM_DEFAULT,
            DATE_LONGDATE,
            &time,
            NULL,
            dateBuffer.data(),
            (int)dateBuffer.capacity(),
            NULL
        );

        GetTimeFormatEx(
            LOCALE_NAME_SYSTEM_DEFAULT,
            0,
            &time,
            NULL,
            timeBuffer.data(),
            (int)timeBuffer.capacity()
        );

        std::wstring dateTime = dateBuffer + L" " + timeBuffer;

        return dateTime;
    }



}