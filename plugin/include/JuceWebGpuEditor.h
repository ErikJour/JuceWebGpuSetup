#pragma once

#include "JuceWebGpuProcessor.h"
#include "WebGpuWindow.h"
#define WIDTH  450
#define HEIGHT 450

//==============================================================================
class JuceWebGpuProcessorEditor final : public juce::AudioProcessorEditor,
                                        private juce::Timer
{
public:
    explicit JuceWebGpuProcessorEditor (JuceWebGpuProcessor&);
    ~JuceWebGpuProcessorEditor() override;

    //==============================================================================
    void parentHierarchyChanged() override;
    void resized() override;
    void timerCallback() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp  (const juce::MouseEvent& event) override;
    void startSequencer();
    void resetSequencer();
    void initializeButton();


    bool mOn = false;
//    AnimatedButton button;

private:

    JuceWebGpuProcessor& processorRef;
    WebGpuWindow                mWebGpuWindow;
#if JUCE_MAC
    juce::NSViewComponent       mNativeView;
#elif JUCE_WINDOWS
    juce::HWNDComponent         mNativeView;
#endif
    double   mStartTimeMs         = 0.0;
    bool     mStartTimeSet        = false;
    uint32_t mConfiguredWidth     = 0;
    uint32_t mConfiguredHeight    = 0;
    uint32_t currentFrame         = 0;
    uint32_t mTick                = 0;

    //Button
    std::unique_ptr<juce::ParameterAttachment> buttonAttachment;  // param→UI binding
    bool buttonOn = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceWebGpuProcessorEditor)
};
