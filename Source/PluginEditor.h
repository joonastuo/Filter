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
    FilterAudioProcessorEditor (FilterAudioProcessor&, AudioProcessorValueTreeState&);
    ~FilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    FilterAudioProcessor& processor;
	AudioProcessorValueTreeState& mParameters;

	Slider mFcSlider;
	Slider mGainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
