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
    : AudioProcessorEditor (&p), processor (p), mParameters(vt)
{
    setSize (400, 300);

	mFcSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);

	addAndMakeVisible(mFcSlider);
	addAndMakeVisible(mGainSlider);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void FilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
