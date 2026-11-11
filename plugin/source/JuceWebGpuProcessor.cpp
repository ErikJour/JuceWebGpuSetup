#include "../include/JuceWebGpuProcessor.h"
#include "../include/JuceWebGpuEditor.h"



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
    mSequencerClick = 0;
    mCount = 0;

    castParameter(apvts, ParameterID::strikeNote, strikeNoteParam);
    castParameter(apvts, ParameterID::sequencerOn, sequencerOnParam);
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
    uint32_t bpm = 80;
    mOscillator.setSampleRate(sampleRate);
    mTickAdvance = static_cast<uint32_t>(sampleRate * 60 / bpm); //33075 samples
    mNoteLengthInSamples = 25000;
    mOscillator.setFrequency(300.0f);
    parametersChanged.store(true);
    juce::ignoreUnused ( samplesPerBlock);
    mSampleRate = sampleRate;
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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bool expected = true;
    if (parametersChanged.compare_exchange_strong(expected, false))
        update();


    if (mSeqOn) { processSequencer(buffer); }
}

void JuceWebGpuProcessor::processSequencer(juce::AudioBuffer<float>& buffer)
{
    //First note or 0 index is messed up

    //Looping through incoming samples
    for (int i = 0; i < buffer.getNumSamples(); i++) {

        //Our sequencer is turned on
        if (mSeqOn) {
            //sampleTick equals 33075
            if (mSampleTick == mTickAdvance) {
                mSequencerClick++;
                //Voice note on
                mOscillator.processBuffer(buffer.getWritePointer(0), buffer.getNumSamples());

                //Increase the click to the backend
                if (mSequencerClick > 5) { mSequencerClick = 0; }
                mSampleTick = 0;
            }
            if (mSampleTick == 0) {
                mSampleTick = 0;
                //voice note off
                mOscillator.reset();
            }
            mSampleTick++;
        } else {
            mSampleTick = 0;
            mOscillator.reset();
        }
    }
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

    bool onOff         = strikeNoteParam->get();
    mStrikeNote        = onOff;

    bool seqOnOff      = sequencerOnParam->get();
    mSeqOn             = seqOnOff;
}


//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceWebGpuProcessor();
}
