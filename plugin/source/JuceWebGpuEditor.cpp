#include "../include/JuceWebGpuProcessor.h"
#include "../include/JuceWebGpuEditor.h"

//==============================================================================
JuceWebGpuProcessorEditor::JuceWebGpuProcessorEditor (JuceWebGpuProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (WIDTH, HEIGHT);
    mWebGpuWindow.initialize();
    resetSequencer();
    initializeButton();
    initializeStrikeNote();
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

//    button.initializeButton();

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
        mWebGpuWindow.getScene().renderFrame((float) elapsed);
    }
        startSequencer();
}

void JuceWebGpuProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    //Get our current x and y mouse positions
    const auto xPos = static_cast<uint32_t>(event.x);
    const auto yPos = static_cast<uint32_t>(event.y);

    //Set the click for the webgpu scene
    mWebGpuWindow.getScene().setClick(xPos, yPos);

    //Set the click for the audio processor
    mButtonOn = ! mButtonOn;
    buttonAttachment->setValueAsCompleteGesture(mButtonOn);
}

void JuceWebGpuProcessorEditor::mouseUp(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);
}

void JuceWebGpuProcessorEditor::resized() {}

void JuceWebGpuProcessorEditor::startSequencer()
{
    //Pass this to our webgpu window
    mTick = static_cast<uint32_t>(processorRef.mSequencerClick.load(std::memory_order_relaxed));
    mWebGpuWindow.getScene().getSequencerInterval(mTick);
}

void JuceWebGpuProcessorEditor::resetSequencer()
{
    mTick = 0;
    mWebGpuWindow.getScene().getSequencerInterval(mTick);
}

void JuceWebGpuProcessorEditor::initializeButton()
{
    //============================================================
    //Button setup
    //============================================================
    //Get a pointer to the button parameter
    auto* button = processorRef.apvts.getParameter(ParameterID::sequencerOn.getParamID());
    jassert(button != nullptr);

    //Set our parameter attachment and callback function
    buttonAttachment = std::make_unique<juce::ParameterAttachment>(
            *button,
            [this](const bool newValue) { mButtonOn = newValue; });
    //Send initial update
    buttonAttachment->sendInitialUpdate();
}

void JuceWebGpuProcessorEditor::initializeStrikeNote()
{
    //============================================================
    //Note setup
    //============================================================
    //Get a pointer to the button parameter
    auto* strikeNote = processorRef.apvts.getParameter(ParameterID::strikeNote.getParamID());
    jassert(strikeNote != nullptr);

    //Set our parameter attachment and callback function
    strikeNoteAttachment = std::make_unique<juce::ParameterAttachment>(
            *strikeNote,
            [this](const bool newValue) { mStrikeNote = newValue; });
    //Send initial update
    strikeNoteAttachment->sendInitialUpdate();
}

void JuceWebGpuProcessorEditor::mouseWheelMove(const juce::MouseEvent& e,
                                                  const juce::MouseWheelDetails& wheel)
{
    juce::ignoreUnused(e);
    mWebGpuWindow.getScene().onScroll(wheel.deltaX, wheel.deltaY);
}

