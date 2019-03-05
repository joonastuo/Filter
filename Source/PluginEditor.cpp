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
    //setSize (140, 140);
	// Initialise GUI elements
	initialiseGUI();
	this->setResizable(true, true);
	this->setResizeLimits(180, 180, 500, 320);
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
	auto area = getLocalBounds().reduced(10.f, 10.f);

	float knobWidth = 60.f;
	float knobHeight = 60.f;
	float labelHeight = mLabelFontSize;
	float buttonWidth = (2.f * knobWidth) / 3.f;
	float buttonHeight = 30.f;

	FlexBox fcBox;
	fcBox.justifyContent = FlexBox::JustifyContent::center;
	fcBox.alignContent = FlexBox::AlignContent::center;
	fcBox.flexDirection = FlexBox::Direction::column;
	fcBox.items.addArray({ FlexItem(mFcLabel).withWidth(knobWidth).withHeight(labelHeight),
						   FlexItem(mFcSlider).withWidth(knobWidth).withHeight(knobHeight) });

	FlexBox resBox;
	resBox.justifyContent = FlexBox::JustifyContent::center;
	resBox.alignContent = FlexBox::AlignContent::center;
	resBox.flexDirection = FlexBox::Direction::column;
	resBox.items.addArray({ FlexItem(mResLabel).withWidth(knobWidth).withHeight(labelHeight),
						    FlexItem(mResSlider).withWidth(knobWidth).withHeight(knobHeight) }); 
	FlexBox sliderBox;
	sliderBox.justifyContent = FlexBox::JustifyContent::center;
	sliderBox.alignContent = FlexBox::AlignContent::center;
	sliderBox.flexDirection = FlexBox::Direction::row;
	sliderBox.items.addArray({FlexItem(fcBox).withHeight(labelHeight + knobHeight).withWidth(knobWidth), 
							  FlexItem(resBox).withHeight(labelHeight + knobHeight).withWidth(knobWidth)});

	FlexBox buttonBox;
	buttonBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	buttonBox.alignContent = FlexBox::AlignContent::center;
	buttonBox.flexDirection = FlexBox::Direction::row;
	buttonBox.items.addArray({ FlexItem(mLPButton).withWidth(buttonWidth).withHeight(buttonHeight),
							   FlexItem(mHPButton).withWidth(buttonWidth).withHeight(buttonHeight),
							   FlexItem(mBPButton).withWidth(buttonWidth).withHeight(buttonHeight) });

	float spaceBetween = 20.f;
	float parameterWidth = 2 * knobWidth;
	float parameterHeight = labelHeight + knobHeight + buttonHeight + spaceBetween;
	
	FlexBox parameterBox;
	parameterBox.justifyContent = FlexBox::JustifyContent::center;
	parameterBox.alignContent = FlexBox::AlignContent::spaceBetween;
	parameterBox.flexDirection = FlexBox::Direction::column;
	parameterBox.items.addArray({FlexItem(sliderBox).withWidth(parameterWidth).withHeight(knobHeight + labelHeight),
							     FlexItem(buttonBox).withWidth(parameterWidth).withHeight(buttonHeight + spaceBetween)});
	
	FlexBox masterBox;
	masterBox.justifyContent = area.getWidth() - parameterWidth > 1.4 * area.getHeight() ? FlexBox::JustifyContent::spaceAround : FlexBox::JustifyContent::center;
	masterBox.alignContent = FlexBox::AlignContent::center;
	masterBox.flexDirection = FlexBox::Direction::row;
	if (area.getWidth() - parameterWidth > 1.4 * area.getHeight())
		masterBox.items.addArray({ FlexItem(mMagView).withWidth(getWidth() - parameterWidth - 40.f).withHeight(area.getHeight()),
								   FlexItem(parameterBox).withWidth(parameterWidth).withHeight(parameterHeight) });
	else
		masterBox.items.addArray({ FlexItem(parameterBox).withWidth(parameterWidth).withHeight(parameterHeight) });

	masterBox.performLayout(area);
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
	mFcLabel.setFont(mLabelFontSize);
	mResLabel.setFont(mLabelFontSize);
	mSelectLabel.setFont(mLabelFontSize);
	addAndMakeVisible(mResLabel);
	addAndMakeVisible(mFcLabel);
	addAndMakeVisible(mSelectLabel);
	addAndMakeVisible(mMagView);
	// Set up buttons
	mLPButton.setSize(30.f, 20.f);
	mLPButton.setLookAndFeel(&mLPButtonLookAndFeel);
	addAndMakeVisible(mLPButton);
	mHPButton.setSize(30.f, 20.f);
	mHPButton.setLookAndFeel(&mHPButtonLookAndFeel);
	addAndMakeVisible(mHPButton);
	mBPButton.setSize(30.f, 20.f);
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
