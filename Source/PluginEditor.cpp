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
    : AudioProcessorEditor (&p), processor (p), mParameters(p.getState())
{
    setSize (400, 250);
	initialiseGUI();
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor() {}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) 
{
	g.fillAll(Colour(40, 190, 127).darker(.4));
	g.setColour(Colours::white);
	float xCenter = (getWidth() / 4.f) + 10.f;
	float yCenter = getHeight() - (getHeight() - 60.f) / 2.f;
	float r = 52;
	xCenter = getWidth() - xCenter;
}

void FilterAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(20, 20);
	mSelectLabel.setBounds(area.removeFromTop(20.f));
	area.removeFromTop(5.f);
	auto selectArea = area.removeFromTop(20);
	selectArea.removeFromLeft(40.f);
	selectArea.removeFromRight(40.f);
	mSelectFilter.setBounds(selectArea);
	area.removeFromTop(10.f);
	auto labelArea = area.removeFromTop(20.f);
	mFcLabel.setBounds(labelArea.removeFromLeft(area.getWidth() * .5f));
	mResLabel.setBounds(labelArea);
	area.removeFromTop(5.f);
	auto bottomLeft = area.removeFromLeft(area.getWidth() * .5f);
	mFcSlider.setBounds(bottomLeft);
	mResSlider.setBounds(area);
}

void FilterAudioProcessorEditor::initialiseGUI()
{
	// Set up sliders
	mFcSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mResSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mFcSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,true, 100, 20);
	mResSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);
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
	mSelectFilter.setJustificationType(Justification::centred);
	addAndMakeVisible(mSelectFilter);
	// Set up Labels
	mFcLabel.setText("Freq", dontSendNotification);
	mResLabel.setText("Res", dontSendNotification);
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
}
