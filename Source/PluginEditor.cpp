/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p, AudioProcessorValueTreeState& vt)
    : AudioProcessorEditor (&p), processor (p), mParameters(vt), mMagView(vt)
{
    setSize (400, 300);

	mFcSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mFcSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,true, 100, 20);
	mGainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);

	addAndMakeVisible(mFcSlider);
	addAndMakeVisible(mGainSlider);
	addAndMakeVisible(mMagView);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) {}

void FilterAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(10, 10);

	FlexBox controlBox;
	controlBox.justifyContent = FlexBox::JustifyContent::center;
	controlBox.alignContent = FlexBox::AlignContent::center;
	controlBox.flexDirection = FlexBox::Direction::row;
	int sliderWidth = getWidth() / 2;
	int sliderHeight = 100;
	controlBox.items.addArray({ splitComponent(mFcSlider, sliderWidth, sliderHeight),
								splitComponent(mGainSlider, sliderWidth, sliderHeight)});

	FlexBox masterBox;
	masterBox.justifyContent = FlexBox::JustifyContent::center;
	masterBox.alignContent = FlexBox::AlignContent::center;
	masterBox.flexDirection = FlexBox::Direction::column;
	masterBox.items.addArray({splitVertical(mMagView, getHeight() * 1/2),
							  FlexItem(controlBox).withWidth(getWidth()).withHeight(getHeight() * 1/2)});
	masterBox.performLayout(area);
}

FlexItem FilterAudioProcessorEditor::splitComponent(Component & c, int width, int height)
{
	return FlexItem(c).withWidth(width).withHeight(height);
}

FlexItem FilterAudioProcessorEditor::splitVertical(Component& c, int height)
{
	return FlexItem(c).withWidth(getWidth()).withHeight(height);
}

FlexItem FilterAudioProcessorEditor::splitHorizontal(Component& c, int width)
{
	return FlexItem(c).withHeight(100).withWidth(width);
}
