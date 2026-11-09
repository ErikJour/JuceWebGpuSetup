#include "../include/JuceWebGpuProcessor.h"
#include "../include/JuceWebGpuEditor.h"

//==============================================================================
JuceWebGpuProcessorEditor::JuceWebGpuProcessorEditor (JuceWebGpuProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (WIDTH, HEIGHT);
    mWebGpuWindow.initialize();
    currentFrame = 0;
    resetSequencer();
}

JuceWebGpuProcessorEditor::~JuceWebGpuProcessorEditor()
{
    stopTimer();
    mWebGpuWindow.terminate();
}

void JuceWebGpuProcessorEditor::parentHierarchyChanged()
{
    //Call the base class function
    AudioProcessorEditor::parentHierarchyChanged();

    //We already have a surface, or we don't have a native window yet
    if (mWebGpuWindow.hasSurface() || getPeer() == nullptr)
        return;

    //Get the display set by the OS
    const auto* webGpuDisplay = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    const double scale                       = webGpuDisplay ? webGpuDisplay->scale : 1.0;
    const auto width                         = static_cast<uint32_t>(getWidth() * scale);
    const auto height                        = static_cast<uint32_t>(getHeight() * scale);

    //Return if surface setup fails
    if (!mWebGpuWindow.initSurface(scale, width, height))
        return;

    //Mouse clicks pass into the JUCE editor
    mNativeView.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(&mNativeView);

    //Platform specific window setup
#if JUCE_MAC
    mNativeView.setView(mWebGpuWindow.getNativeView());
#elif JUCE_WINDOWS
    mNativeView.setHWND(mWebGpuWindow.getNativeView());
#endif
    mNativeView.setBounds(getLocalBounds());

    //Start timer
    mStartTimeMs      = juce::Time::getMillisecondCounterHiRes();
    mStartTimeSet     = true;
    mConfiguredWidth  = width;
    mConfiguredHeight = height;
    startTimerHz(60);
    setResizable (false, false);
}

void JuceWebGpuProcessorEditor::timerCallback()
{
    if (mStartTimeSet) {
        const double elapsed = (juce::Time::getMillisecondCounterHiRes() - mStartTimeMs) * 0.001;
        mWebGpuWindow.getScene().renderFrame(elapsed);
    }

    if (mWebGpuWindow.getScene().buttonClicked) {

        if (currentFrame == 0) {
            startSequencer(); } {

            if (currentFrame < 59) {
                processorRef.mStrikeNote = true;
                currentFrame++;

            } else {

                processorRef.mStrikeNote = false;
                currentFrame = 0;

                }
            }

        } else {
            currentFrame = 0;
            processorRef.mStrikeNote = false;
    }
}

void JuceWebGpuProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    const auto xPos = static_cast<uint32_t>(event.x);
    const auto yPos = static_cast<uint32_t>(event.y);

    mWebGpuWindow.getScene().setClick(xPos, yPos);
}

void JuceWebGpuProcessorEditor::mouseUp(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);
}

void JuceWebGpuProcessorEditor::resized() {}

void JuceWebGpuProcessorEditor::startSequencer()
{
    mTick = processorRef.mSequencerClick;
    mWebGpuWindow.getScene().getSequencerInterval(mTick);
    processorRef.mStrikeNote = true;
}

void JuceWebGpuProcessorEditor::resetSequencer()
{
    mTick = 0;
    mWebGpuWindow.getScene().getSequencerInterval(mTick);
}


