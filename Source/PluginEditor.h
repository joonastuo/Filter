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
	void paintMarkers(Graphics&, float xCenter, float yCenter, float r);
    void resized() override;
	void initialiseGUI();
private:
    FilterAudioProcessor& processor;
	AudioProcessorValueTreeState& mParameters;

	// GUI elements
	Label mFcLabel;
	Label mResLabel;
	Label mSelectLabel;

	Slider mFcSlider;
	Slider mResSlider;

	ComboBox mSelectFilter;
	// Attachments
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
	
	std::unique_ptr<SliderAttachment> mFcAttachment;
	std::unique_ptr<SliderAttachment> mResAttachment;
	std::unique_ptr<ComboBoxAttachment> mSelectAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
