#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../utils/AudioUtils.h"
#include "../dsp/Oscillator.h"
#include "WebGPUSynth.h"

namespace ParameterID {
#define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);

    PARAMETER_ID(strikeNote)
    PARAMETER_ID(sequencerOn)

#undef PARAMETER_ID
}

//==============================================================================
class JuceWebGpuProcessor final : public juce::AudioProcessor,
                                    public juce::ValueTree::Listener
{
public:
    //==============================================================================
    JuceWebGpuProcessor();
    ~JuceWebGpuProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void processSequencer(juce::AudioBuffer<float>& buffer);

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts {*this,
                                              nullptr,
                                              "Parameters",
                                              createParameterLayout() };
    bool mStrikeNote {false};
    bool mSeqOn {false};
    uint32_t mSequencerClick      = 0;
    uint32_t mNoteLengthInSamples = 25000;
    uint32_t mCount               = 0;

private:
    Oscillator mOscillator{};
    juce::AudioParameterBool*  strikeNoteParam{};
    juce::AudioParameterBool*  sequencerOnParam{};
    double mSampleRate    = 44100;
    uint32_t mTickAdvance = 0;
    uint32_t mSampleTick  = 0;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override { parametersChanged.store(true); }
    void update();
    std::atomic<bool> parametersChanged{false};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceWebGpuProcessor)
};
