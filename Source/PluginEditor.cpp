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
    setSize (300, 300);
	initialiseGUI();
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor() {}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) 
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); 
	g.setColour(Colours::white);
	float xCenter = (getWidth() / 4.f) + 10.f;
	float yCenter = getHeight() - (getHeight() - 60.f) / 2.f;
	float r = 52;
	xCenter = getWidth() - xCenter;
}

void FilterAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(40, 20);
	auto magArea = area.removeFromTop(100.f);
	magArea.removeFromLeft(20.f);
	magArea.removeFromRight(20.f);
	mMagView.setBounds(magArea);
	area.removeFromTop(10.f);
	auto mFreqArea = area.removeFromLeft(.25*getWidth());
	auto mResArea = area.removeFromLeft(.25*getWidth());
	mFcLabel.setBounds(mFreqArea.removeFromBottom(20.f));
	mFcSlider.setBounds(mFreqArea);
	mResLabel.setBounds(mResArea.removeFromBottom(20.f));
	mResSlider.setBounds(mResArea);
	mSelectLabel.setBounds(area.removeFromBottom(20.f));
	area.removeFromBottom(40.f);
	mSelectFilter.setBounds(area.removeFromBottom(25.f));
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
	mSelectAttachment.reset(new ComboBoxAttachment(mParameters, "selectFilter", mSelectFilter));
	mFcSlider.setSkewFactorFromMidPoint(1000.0);
	// Set up combo box	
	mSelectFilter.addItemList({ "Low Pass", "High Pass", "Band Pass" }, 1);
	float selectFilter = *mParameters.getRawParameterValue("selectFilter");
	mSelectFilter.setSelectedItemIndex(selectFilter, true);
	mSelectFilter.setJustificationType(Justification::centred);
	addAndMakeVisible(mSelectFilter);
	// Set up Labels
	mFcLabel.setText("Frequency", dontSendNotification);
	mResLabel.setText("Resonance", dontSendNotification);
	mSelectLabel.setText("Filter Type",dontSendNotification);
	mFcLabel.setJustificationType(Justification::centred);
	mResLabel.setJustificationType(Justification::centred);
	mSelectLabel.setJustificationType(Justification::centred);
	mFcLabel.setFont(18);
	mResLabel.setFont(18);
	mSelectLabel.setFont(18);
	addAndMakeVisible(mResLabel);
	addAndMakeVisible(mFcLabel);
	addAndMakeVisible(mSelectLabel);
	addAndMakeVisible(mMagView);
}
