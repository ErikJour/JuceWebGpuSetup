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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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
    mOscillator.setFrequency(300.0f);
    parametersChanged.store(true);
    juce::ignoreUnused ( samplesPerBlock);
}

void JuceWebGpuProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool JuceWebGpuProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
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
    bool noteOn = mStrikeNote;
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bool expected = true;
    if (parametersChanged.compare_exchange_strong(expected, false))
        update();

    for (int i = 0; i < buffer.getNumSamples(); i++) {

        if (mSampleTick % mTickAdvance == 0) {
            if (noteOn) {
                if (mSequencerClick < 5) {
                    mSequencerClick++;
                } else {
                    mSequencerClick = 0;
                }
            }

        }
        mSampleTick++;
    }


    if (noteOn) { mOscillator.processBuffer(buffer.getWritePointer(0), buffer.getNumSamples()); }
    else        { mOscillator.reset(); }
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

    std::cout << "Sequencer on / off in update: " << mSeqOn << std::endl;

}

void JuceWebGpuProcessor::setSequencer(bool on)
{
    bool seqOnOff      = sequencerOnParam->get();
    seqOnOff = on;
    mSeqOn             = seqOnOff;
    std::cout << mSeqOn << std::endl;
}


//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceWebGpuProcessor();
}
