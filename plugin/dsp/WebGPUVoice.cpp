//
// Created by Erik Jourgensen on 5/19/26.
//
#include "WebGPUVoice.h"

WebGPUVoice::WebGPUVoice() : mSampleRate(0) { }

void WebGPUVoice::reset(const double sampleRate)
{
    mSampleRate     = sampleRate;
    note            = 0;

    mEnvelope.reset();
}

void WebGPUVoice::render(float* buffer, const int sampleCount)
{

    mEnvelope.process(buffer, sampleCount);
}

void WebGPUVoice::release() { mEnvelope.release(); }

