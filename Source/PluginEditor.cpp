/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), mParameters(p.getState()), mMagView(p.getState())
{
    setSize (400, 300);

	mFcSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mFcSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,true, 100, 20);
	mGainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);

	mFcAttachment.reset(new SliderAttachment(mParameters, "fc", mFcSlider));
	mGainAttachment.reset(new SliderAttachment(mParameters, "gain", mGainSlider));

	addAndMakeVisible(mFcSlider);
	addAndMakeVisible(mGainSlider);
	addAndMakeVisible(mMagView);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) 
{
}

void FilterAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(20, 20);

	mMagView.setBounds(area.removeFromTop(100.f));
	area.removeFromTop(10.f);
	auto bottomLeft = area.removeFromLeft(area.getWidth() * .5f);
	mFcSlider.setBounds(bottomLeft);
	mGainSlider.setBounds(area);
}

FlexItem FilterAudioProcessorEditor::splitSliderComponent(Component & c)
{
	return FlexItem(c).withMinWidth(100.f).withMinHeight(100.f);
}

