#include "../include/JuceWebGpuProcessor.h"
#include "../include/JuceWebGpuEditor.h"

#define BPM 80

//==============================================================================
JuceWebGpuProcessor::JuceWebGpuProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    apvts.state.addListener(this);

    //Initialize sequencer click, count, and samplerate
    mSequencerClick = 0;
    mSampleRate     = 48000;

    //Pass apvts by reference, get the param id, store in our member pointer
    castParameter(apvts, ParameterID::strikeNote, mStrikeNoteParam);
    castParameter(apvts, ParameterID::sequencerOn, mSequencerOnParam);
}

JuceWebGpuProcessor::~JuceWebGpuProcessor()
{
    apvts.state.removeListener(this);
    mOscillator.reset();
}

//==============================================================================
const juce::String JuceWebGpuProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceWebGpuProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceWebGpuProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuceWebGpuProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuceWebGpuProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceWebGpuProcessor::getNumPrograms()
{
    return 1;
}

int JuceWebGpuProcessor::getCurrentProgram()
{
    return 0;
}

void JuceWebGpuProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String JuceWebGpuProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void JuceWebGpuProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void JuceWebGpuProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused ( samplesPerBlock);
    mSampleRate = sampleRate;

    //Our tick should advance every 80 times per minute, or every 33075 samples
    mTickAdvance = static_cast<uint32_t>(sampleRate * 60 / BPM);

    //Oscillator prepare
    mOscillator.setFrequency(300.0f);
    mOscillator.setSampleRate(sampleRate);

    parametersChanged.store(true);
}

void JuceWebGpuProcessor::releaseResources(){}

bool JuceWebGpuProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void JuceWebGpuProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused ( midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bool expected = true;
    if (parametersChanged.compare_exchange_strong(expected, false))
        update();

    //If the sequencer is on, hand the buffer to the sequencer, otherwise reset the state
    mSeqOn = mSequencerOnParam->get();
    if (mSeqOn) { processSequencer(buffer); }
    else        { resetSequencer(buffer); }
}

//==============================================================================
bool JuceWebGpuProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* JuceWebGpuProcessor::createEditor()
{
    return new JuceWebGpuProcessorEditor (*this);
}

//==============================================================================
void JuceWebGpuProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void JuceWebGpuProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessorValueTreeState::ParameterLayout JuceWebGpuProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterBool>(
            ParameterID::strikeNote,
            "On",
            false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
            ParameterID::sequencerOn,
            "Sequencer on",
            false));

    return layout;
}

void JuceWebGpuProcessor::update() {

    bool onOff         = mStrikeNoteParam->get();
    mStrikeNote        = onOff;

    bool seqOnOff      = mSequencerOnParam->get();
    mSeqOn             = seqOnOff;
}

void JuceWebGpuProcessor::processSequencer (juce::AudioBuffer<float>& buffer)
{
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        if (++mSampleTick >= mTickAdvance)
        {
            mSampleTick = 0;
            mSequencerClick.store ((mSequencerClick.load (std::memory_order_relaxed) + 1) % 6,
                                   std::memory_order_relaxed);
        }
    }
}

void JuceWebGpuProcessor::resetSequencer(juce::AudioBuffer<float>& buffer)
{
    mSequencerClick = 0;
    mSampleTick     = 0;
    mOscillator.reset();
    buffer.clear();
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceWebGpuProcessor();
}
