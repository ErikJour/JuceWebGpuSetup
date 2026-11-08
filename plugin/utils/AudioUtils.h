//
// Created by Erik Jourgensen on 9/2/26.
//

#pragma once
#include "juce_audio_processors/juce_audio_processors.h"

template<typename T>
inline static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T> (apvts.getParameter(id.getParamID()));
    jassert(destination);
}
