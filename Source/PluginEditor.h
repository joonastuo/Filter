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
    FilterAudioProcessor& processor;
	AudioProcessorValueTreeState& mParameters;
	MagView mMagView;
	KnobLookAndFeel knobLookAndFeel;
	LPButtonLookAndFeel mLPButtonLookAndFeel;
	HPButtonLookAndFeel mHPButtonLookAndFeel;
	BPButtonLookAndFeel mBPButtonLookAndFeel;

	// GUI elements
	Label mFcLabel;
	Label mResLabel;
	Label mSelectLabel;

	Slider mFcSlider;
	Slider mResSlider;

	TextButton mLPButton;
	TextButton mHPButton;
	TextButton mBPButton;

	ComboBox mSelectFilter;
	// Attachments
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
	
	std::unique_ptr<SliderAttachment> mFcAttachment;
	std::unique_ptr<SliderAttachment> mResAttachment;
	std::unique_ptr<ComboBoxAttachment> mSelectAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
