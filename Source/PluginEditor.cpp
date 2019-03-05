/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	  mParameters(p.getState()),
	  mMagView(p.getState())
{
	// Plugin size
    setSize (350, 240);
	// Initialise GUI elements
	initialiseGUI();
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor() {}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) 
{
	// Plugin background
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); 
}

void FilterAudioProcessorEditor::resized()
{
	// Plugin area, reduce from sides
	auto area = getLocalBounds().reduced(20, 20);
	// Area of magitude response component
	auto magArea = area.removeFromTop(area.getHeight() / 2.f);
	magArea.removeFromLeft(10.f);
	magArea.removeFromRight(10.f);
	mMagView.setBounds(magArea);

	// Rest of the area for knobs and buttons
	area.removeFromTop(10.f);
	// Width of the knobs
	auto sliderWidth = .25 * area.getWidth();
	// Knob areas
	auto mFreqArea = area.removeFromLeft(sliderWidth);
	auto mResArea = area.removeFromLeft(sliderWidth);
	mFreqArea.removeFromTop(10.f);
	mResArea.removeFromTop(10.f);
	// Set knob and label bounds
	mFcLabel.setBounds(mFreqArea.removeFromTop(16.f));
	mFcSlider.setBounds(mFreqArea);
	mResLabel.setBounds(mResArea.removeFromTop(16.f));
	mResSlider.setBounds(mResArea);
	// Set filter selection buttons
	area.removeFromTop(20.f);
	area.removeFromLeft(15.f);
	// LP button bounds
	mLPButton.setBounds(area.removeFromLeft(40.f));
	area.removeFromLeft(5.f);
	// HP button bounds
	mHPButton.setBounds(area.removeFromLeft(40.f));
	area.removeFromLeft(5.f);
	// BP button bounds
	mBPButton.setBounds(area.removeFromLeft(40.f));
}

void FilterAudioProcessorEditor::initialiseGUI()
{
	// Set up sliders
	mFcSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mResSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mFcSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 1.f, 1.f);
	mResSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 1.f, 1.f);
	addAndMakeVisible(mFcSlider);
	addAndMakeVisible(mResSlider);
	mFcSlider.setLookAndFeel(&knobLookAndFeel);
	mResSlider.setLookAndFeel(&knobLookAndFeel);
	// Slider attachments
	mFcAttachment.reset(new SliderAttachment(mParameters, "fc", mFcSlider));
	mResAttachment.reset(new SliderAttachment(mParameters, "res", mResSlider));
	// fc slider into log10 scale
	mFcSlider.setSkewFactorFromMidPoint(1000.0);

	// Set up Labels
	mFcLabel.setText("Freq", dontSendNotification);
	mResLabel.setText("Res", dontSendNotification);
	mSelectLabel.setText("Type",dontSendNotification);
	mFcLabel.setJustificationType(Justification::centred);
	mResLabel.setJustificationType(Justification::centred);
	mSelectLabel.setJustificationType(Justification::centred);
	mFcLabel.setFont(16);
	mResLabel.setFont(16);
	mSelectLabel.setFont(18);
	addAndMakeVisible(mResLabel);
	addAndMakeVisible(mFcLabel);
	addAndMakeVisible(mSelectLabel);
	addAndMakeVisible(mMagView);
	// Set up buttons
	mLPButton.setSize(40.f, 30.f);
	mLPButton.setLookAndFeel(&mLPButtonLookAndFeel);
	addAndMakeVisible(mLPButton);
	mHPButton.setSize(40.f, 30.f);
	mHPButton.setLookAndFeel(&mHPButtonLookAndFeel);
	addAndMakeVisible(mHPButton);
	mBPButton.setSize(40.f, 30.f);
	mBPButton.setLookAndFeel(&mBPButtonLookAndFeel);
	addAndMakeVisible(mBPButton);
	mLPButton.addListener(this);
	mHPButton.addListener(this);
	mBPButton.addListener(this);
	mLPButton.setClickingTogglesState(true);
	mHPButton.setClickingTogglesState(true);
	mBPButton.setClickingTogglesState(true);
	int filterType = *mParameters.getRawParameterValue("selectFilter");
	if (filterType == 0)
		mLPButton.setToggleState(true, true);
	else if (filterType == 1)
		mHPButton.setToggleState(true, true);
	else
		mBPButton.setToggleState(true, true);
}

void FilterAudioProcessorEditor::buttonClicked(Button* b)
{
	Value filterType = mParameters.getParameterAsValue("selectFilter");
	// Lowpass button
	if (b == &mLPButton)
	{
		filterType = 0;
		if (mLPButton.getToggleState())
		{
			if (mHPButton.getToggleState())
			{
				mHPButton.setToggleState(false, false);
				mLPButton.setToggleState(true, false);
			}
			if (mBPButton.getToggleState())
			{
				mBPButton.setToggleState(false, false);
				mLPButton.setToggleState(true, false);
			}
		}
	}
	if (b == &mHPButton)
	{
		filterType = 1;
		if (mHPButton.getToggleState())
		{
			if (mLPButton.getToggleState())
			{
				mLPButton.setToggleState(false, false);
				mHPButton.setToggleState(true, false);
			}
			if (mBPButton.getToggleState())
			{
				mBPButton.setToggleState(false, false);
				mHPButton.setToggleState(true, false);
			}
		}
	}
	if (b == &mBPButton)
	{
		filterType = 2;
		if (mBPButton.getToggleState())
		{
			if (mLPButton.getToggleState())
			{
				mLPButton.setToggleState(false, false);
				mBPButton.setToggleState(true, false);
			}
			if (mHPButton.getToggleState())
			{
				mHPButton.setToggleState(false, false);
				mBPButton.setToggleState(true, false);
			}
		}
	}
}
