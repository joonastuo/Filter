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
#include "KnobLookAndFeel.h"
#include "MagView.h"
#include "LPButtonLookAndFeel.h"
#include "HPButtonLookAndFeel.h"
#include "BPButtonLookAndFeel.h"

//==============================================================================
/**
*/
class FilterAudioProcessorEditor  : public AudioProcessorEditor, TextButton::Listener
{
public:
    FilterAudioProcessorEditor (FilterAudioProcessor&);
    ~FilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void initialiseGUI();
	void buttonClicked(Button* b) override;
private:
	// Reference to plugin processor
    FilterAudioProcessor& processor;
	// Reference to ValueTree
	AudioProcessorValueTreeState& mParameters;
	// Component class that paints Magnitude response of filter
	MagView mMagView;
	// How much wider the magView window is
	float mMagViewRatio = 2.5f;

	// LookAndFeel-classes to customize slider and button look
	KnobLookAndFeel knobLookAndFeel;
	LPButtonLookAndFeel mLPButtonLookAndFeel;
	HPButtonLookAndFeel mHPButtonLookAndFeel;
	BPButtonLookAndFeel mBPButtonLookAndFeel;

	// GUI elements

	// Labels
	Label mFreqLabel;
	Label mResLabel;
	Label mFilterTypeLabel;

	float mLabelFontSize = 18.f;

	// Sliders
	Slider mFreqSlider;
	Slider mResSlider;

	// Buttons
	TextButton mLPButton;
	TextButton mHPButton;
	TextButton mBPButton;

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	
	// Attachments so that ValueTree parameters update with slier value change
	std::unique_ptr<SliderAttachment> mFreqAttachment;
	std::unique_ptr<SliderAttachment> mResAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
