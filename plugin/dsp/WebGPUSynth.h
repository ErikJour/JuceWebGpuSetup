//
// Created by Erik Jourgensen on 5/19/26.
//

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "WebGPUVoice.h"

class WebGPUSynth
{
public:
    WebGPUSynth();

    void distributeResources(double sampleRate, int samplesPerBlock);
    static void releaseResources();
    void reset(double sampleRate);
    void render(float** outputBuffers, int sampleCount);
    void midiMessages(uint8_t data0, uint8_t data1, uint8_t data2);
    void startVoice(int note, int velocity);
    void noteOn(int note, int velocity);
    void noteOff(int note);
    //======================================================================================
    void setSustain (const float sustain)          { voice.mEnvelope.setSustain(sustain); }
    void setRelease(const float release)
    {
        envRelease = release;
        voice.mEnvelope.setReleaseMultiplier(envRelease);
    }

    float envAttack  {};
    float envDecay   {};
    float envSustain {};
    float envRelease {};

private:
    double     mSampleRate;
    WebGPUVoice voice;

};