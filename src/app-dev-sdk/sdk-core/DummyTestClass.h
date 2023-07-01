#pragma once
#include "DummyTestClass.g.h"

// WARNING: This file is automatically generated by a tool. Do not directly
// add this file to your project, as any changes you make will be lost.
namespace winrt::Microsoft::Devices::Midi2::implementation
{
    struct DummyTestClass : DummyTestClassT<DummyTestClass>
    {
        DummyTestClass() = default;

        static winrt::Microsoft::Devices::Midi2::MidiUmpMessageType GetSomeStaticValue();
        uint32_t SomeProperty();
        void SomeProperty(uint32_t value);
        uint32_t SomeReadOnlyProperty();
        winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::Devices::Midi2::MidiUmpWithTimestamp> Umps();

        void LoadDummyUmps(uint32_t count);

    private:
        winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::Devices::Midi2::MidiUmpWithTimestamp> 
            _umps{ winrt::single_threaded_vector<winrt::Microsoft::Devices::Midi2::MidiUmpWithTimestamp>() };

        uint32_t _someProperty = 0;
    };
}
namespace winrt::Microsoft::Devices::Midi2::factory_implementation
{
    struct DummyTestClass : DummyTestClassT<DummyTestClass, implementation::DummyTestClass>
    {
    };
}