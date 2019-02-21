/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Identifiers.h"
#include "MagView.h"

//==============================================================================
/**
*/
class FilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    FilterAudioProcessorEditor (FilterAudioProcessor&);
    ~FilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	FlexItem splitSliderComponent(Component& c);

private:
    FilterAudioProcessor& processor;
	AudioProcessorValueTreeState& mParameters;
	MagView mMagView;

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Label mFcLabel;
	Label mGainLabel;

	Slider mFcSlider;
	Slider mGainSlider;

	std::unique_ptr<SliderAttachment> mFcAttachment;
	std::unique_ptr<SliderAttachment> mGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
