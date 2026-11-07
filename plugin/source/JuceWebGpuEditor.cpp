#include "../include/JuceWebGpuProcessor.h"
#include "../include/JuceWebGpuEditor.h"

//==============================================================================
JuceWebGpuProcessorEditor::JuceWebGpuProcessorEditor (JuceWebGpuProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (WIDTH, HEIGHT);
    mWebGpuWindow.initialize();
}

JuceWebGpuProcessorEditor::~JuceWebGpuProcessorEditor()
{
    stopTimer();
    mWebGpuWindow.terminate();
}

void JuceWebGpuProcessorEditor::parentHierarchyChanged()
{
    AudioProcessorEditor::parentHierarchyChanged();

    if (mWebGpuWindow.hasSurface() || getPeer() == nullptr)
        return;

    const auto* webGpuDisplay = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    const double scale        = webGpuDisplay ? webGpuDisplay->scale : 1.0;
    const auto width          = static_cast<uint32_t>(getWidth() * scale);
    const auto height         = static_cast<uint32_t>(getHeight() * scale);

    if (!mWebGpuWindow.initSurface(scale, width, height))
        return;
    mNativeView.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(&mNativeView);
#if JUCE_MAC
    mNativeView.setView(mWebGpuWindow.getNativeView());
#elif JUCE_WINDOWS
    mNativeView.setHWND(mWebGpuWindow.getNativeView());
#endif
    mNativeView.setBounds(getLocalBounds());
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
    //I want to pass a flag every 45 frames to my graphics engine to update the sequencer
    //This can also be a juce audio parameter int that triggers specific notes
    if (mWebGpuWindow.getScene().buttonClicked && currentFrame % 45 == 0) {

        mWebGpuWindow.getScene().getSequencerInterval(tick);

        if (tick < 5) { ++tick; }
        else { tick = 0; }
    }
    currentFrame++;
}

void JuceWebGpuProcessorEditor::resized() {}

void JuceWebGpuProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    const auto xPos = static_cast<uint32_t>(event.x);
    const auto yPos = static_cast<uint32_t>(event.y);

    mWebGpuWindow.getScene().setClick(xPos, yPos);

}

