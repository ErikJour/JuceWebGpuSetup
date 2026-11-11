//
// Created by Erik Jourgensen on 5/19/26.
//


#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "Envelope.h"

struct WebGPUVoice {
    WebGPUVoice();
    ~WebGPUVoice() = default;

    void reset(double sampleRate);
    void render(float* buffer, int sampleCount);
    void release();

    Envelope        mEnvelope;

    int note      = 0;
    juce::AudioBuffer<float> mAudioBuffer;
    double mSampleRate;

};