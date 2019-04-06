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
	// Initialise GUI elements
	initialiseGUI();
	this->setResizable(true, true);
	int maxWidth = 600;
	int maxHeight = 400;
	this->setResizeLimits(160, 160, maxWidth, maxHeight);
    setSize (566, 372);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor() 
{
	// Empty destructor
}

//==============================================================================
void FilterAudioProcessorEditor::initialiseGUI()
{
	// MAGNITUDE VIEW ==========================		
	addAndMakeVisible(mMagView);
	
	// FILTER FREQUENCY ========================
	// Label
	mFreqLabel.setText(NAMEs::Freq, dontSendNotification);
	mFreqLabel.setJustificationType(Justification::centred);
	mFreqLabel.setFont(mLabelFontSize);
	addAndMakeVisible(mFreqLabel);

	//Slider
	mFreqSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mFreqSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 14);
	mFreqSlider.setTextValueSuffix(" Hz");
	mFreqSlider.setLookAndFeel(&knobLookAndFeel);
	addAndMakeVisible(mFreqSlider);
	mFreqAttachment.reset(new SliderAttachment(mParameters, IDs::filterFrequency, mFreqSlider));
	mFreqSlider.setSkewFactorFromMidPoint(1000.f);

	// FILTER RESONANCE ========================
	// Label
	mResLabel.setText(NAMEs::Res, dontSendNotification);
	mResLabel.setJustificationType(Justification::centred);
	mResLabel.setFont(mLabelFontSize);
	addAndMakeVisible(mResLabel);

	//Slider
	mResSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mResSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 14);
	mResSlider.setLookAndFeel(&knobLookAndFeel);
	addAndMakeVisible(mResSlider);
	mResAttachment.reset(new SliderAttachment(mParameters, IDs::resonance, mResSlider));

	// FILTER TYPE =============================
	// Label
	mFilterTypeLabel.setText(NAMEs::Type,dontSendNotification);
	mFilterTypeLabel.setJustificationType(Justification::centred);
	mFilterTypeLabel.setFont(mLabelFontSize);
	addAndMakeVisible(mFilterTypeLabel);

	// Buttons
	int buttonWidth  = 20;
	int buttonHeight = 15;
	// LP
	mLPButton.setSize(buttonWidth, buttonHeight);
	mLPButton.setLookAndFeel(&mLPButtonLookAndFeel);
	mLPButton.addListener(this);
	mLPButton.setClickingTogglesState(true);
	addAndMakeVisible(mLPButton);
	// HP
	mHPButton.setSize(buttonWidth, buttonHeight);
	mHPButton.setLookAndFeel(&mHPButtonLookAndFeel);
	mHPButton.addListener(this);
	mHPButton.setClickingTogglesState(true);
	addAndMakeVisible(mHPButton);
	// BP
	mBPButton.setSize(buttonWidth, buttonHeight);
	mBPButton.setLookAndFeel(&mBPButtonLookAndFeel);
	mBPButton.addListener(this);
	mBPButton.setClickingTogglesState(true);
	addAndMakeVisible(mBPButton);

	// Initial filter type
	int filterType = static_cast<int>(*mParameters.getRawParameterValue(IDs::filterType));
	switch (filterType)
	{
	case lowpass:  mLPButton.setToggleState(true, sendNotification); break;
	case highpass: mHPButton.setToggleState(true, sendNotification); break;
	case bandpass: mBPButton.setToggleState(true, sendNotification); break;
	default:
		break;
	}
}
//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) 
{
	// Plugin background
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); 
}

//==============================================================================
void FilterAudioProcessorEditor::resized()
{
	// GUI parameters===============================================

	// GUI area
	auto area = getLocalBounds().reduced(10, 10);

	// MagView
	float magViewRatio = mMagViewRatio;
	float magViewMinHeight = 80.f;
	float magViewMinWidth = magViewMinHeight * magViewRatio;
	
	//Knobs
	float knobWidth = 60.f;
	float knobHeight = knobWidth;
	float labelHeight = mLabelFontSize;

	// Buttons
	float buttonWidth = ((2.f * knobWidth) / 3.f) - 6.f;
	float buttonHeight = buttonWidth / 1.6f;

	// parameterBox = knobs + buttons
	float parameterWidth = 2 * knobWidth;
	float spaceBetween = 20.f;
	float parameterHeight = labelHeight + knobHeight + buttonHeight + spaceBetween;
	
	// Portrait or landscape mode (true if portrait)
	bool isPortrait = area.getHeight() * 1.8f > area.getWidth();
	// Is magView shown (portrait & landscape conditions)
	bool magViewPortraitCondition = area.getHeight() - parameterHeight > magViewMinHeight;
	bool magViewLandscapeCondition = area.getWidth() - parameterWidth - spaceBetween > magViewMinWidth;

	float magViewPortraitWidth = static_cast<float>(area.getWidth());
	float magViewPortraitHeight = magViewPortraitWidth / magViewRatio;

	float magViewLandscapeWidth = area.getWidth() - parameterWidth - spaceBetween;
	float magViewCurrHeightLandspace = magViewLandscapeWidth / magViewRatio;
	float magViewLandscapeHeight = magViewCurrHeightLandspace < area.getHeight() ? magViewCurrHeightLandspace : area.getHeight();

	// Perform layout with FlexBox==================================
	// Frequency knob
	FlexBox fcBox;
	fcBox.justifyContent = FlexBox::JustifyContent::center;
	fcBox.alignContent   = FlexBox::AlignContent::center;
	fcBox.flexDirection  = FlexBox::Direction::column;
	fcBox.items.addArray({ FlexItem(mFreqLabel).withWidth(knobWidth).withHeight(labelHeight),
						   FlexItem(mFreqSlider).withWidth(knobWidth).withHeight(knobHeight) });
	// Resonance knob
	FlexBox resBox;
	resBox.justifyContent = FlexBox::JustifyContent::center;
	resBox.alignContent   = FlexBox::AlignContent::center;
	resBox.flexDirection  = FlexBox::Direction::column;
	resBox.items.addArray({ FlexItem(mResLabel).withWidth(knobWidth).withHeight(labelHeight),
						    FlexItem(mResSlider).withWidth(knobWidth).withHeight(knobHeight) }); 

	// Both kobs and their labels
	FlexBox sliderBox;
	sliderBox.justifyContent = FlexBox::JustifyContent::center;
	sliderBox.alignContent   = FlexBox::AlignContent::center;
	sliderBox.flexDirection  = FlexBox::Direction::row;
	sliderBox.items.addArray({ FlexItem(fcBox).withHeight(labelHeight + knobHeight).withWidth(knobWidth), 
							   FlexItem(resBox).withHeight(labelHeight + knobHeight).withWidth(knobWidth)});

	// Filter type buttons
	FlexBox buttonBox;
	buttonBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	buttonBox.alignContent   = FlexBox::AlignContent::center;
	buttonBox.flexDirection  = FlexBox::Direction::row;
	buttonBox.items.addArray({ FlexItem(mLPButton).withWidth(buttonWidth).withHeight(buttonHeight),
							   FlexItem(mHPButton).withWidth(buttonWidth).withHeight(buttonHeight),
							   FlexItem(mBPButton).withWidth(buttonWidth).withHeight(buttonHeight) });

	// Parameter control box (knobs + buttons)
	FlexBox parameterBox;
	parameterBox.justifyContent = FlexBox::JustifyContent::center;
	parameterBox.alignContent   = FlexBox::AlignContent::center;
	parameterBox.flexDirection  = FlexBox::Direction::column;
	parameterBox.items.addArray({ FlexItem(sliderBox).withWidth(parameterWidth).withHeight(knobHeight + labelHeight),
							      FlexItem(buttonBox).withWidth(parameterWidth).withHeight(buttonHeight + spaceBetween)});
	
	// Master box that lays out all the components in  the window
	FlexBox masterBox;
	FlexBox magBox;
	magBox.justifyContent = FlexBox::JustifyContent::center;
	magBox.alignContent   = FlexBox::AlignContent::center;
	magBox.flexDirection  = FlexBox::Direction::column;

	// Portrait mode
	if (isPortrait)
	{

		// masterBox settings
		masterBox.alignContent   = FlexBox::AlignContent::center;
		masterBox.justifyContent = FlexBox::JustifyContent::center;
		masterBox.flexDirection  = FlexBox::Direction::column;
		float parameterBoxSpace = 20.f;

		if (magViewPortraitCondition)
		{
			// MagView box
			magBox.items.add(FlexItem(mMagView).withWidth (magViewPortraitWidth).withHeight (magViewPortraitHeight));
			
			mMagView.setVisible(true);
			masterBox.items.addArray({ FlexItem(magBox).withWidth(static_cast<float>(area.getWidth())).withHeight(magViewPortraitHeight),
									   FlexItem(parameterBox).withWidth(static_cast<float>(area.getWidth())).withHeight(parameterHeight + parameterBoxSpace) });
		}
		else
		{
			mMagView.setVisible(false);
			masterBox.items.addArray({ FlexItem(parameterBox).withWidth(parameterWidth).withHeight(parameterHeight) });
		}
	}
	// Landscape mode
	else
	{

		// masterBox settings
		masterBox.alignContent   = FlexBox::AlignContent::spaceAround;
		masterBox.justifyContent = magViewLandscapeCondition ? FlexBox::JustifyContent::spaceBetween : FlexBox::JustifyContent::center;
		masterBox.flexDirection  = FlexBox::Direction::row;

		float parameterBoxSpace = 20.f;

		if (magViewLandscapeCondition)
		{
			mMagView.setVisible(true);
			// MagView box
			magBox.items.add(FlexItem(mMagView).withWidth (magViewLandscapeWidth).withHeight (magViewLandscapeHeight));
			
			masterBox.items.addArray({ FlexItem(magBox).withWidth(magViewLandscapeWidth).withHeight(static_cast<float>(area.getHeight())),
									   FlexItem(parameterBox).withWidth(parameterWidth + parameterBoxSpace).withHeight(static_cast<float>(area.getHeight())) });
		}
		else
		{
			mMagView.setVisible(false);
			masterBox.items.addArray({ FlexItem(parameterBox).withWidth(parameterWidth).withHeight(static_cast<float>(area.getHeight())) });
		}
	}
	// Perform layout
	masterBox.performLayout(area.toFloat());
}

//==============================================================================
void FilterAudioProcessorEditor::buttonClicked(Button* b)
{
	Value filterType = mParameters.getParameterAsValue(IDs::filterType);
	// Lowpass button
	if (b == &mLPButton)
	{
		filterType = FilterType::lowpass;
		if (mLPButton.getToggleState())
		{
			if (mHPButton.getToggleState())
			{
				mHPButton.setToggleState(false, dontSendNotification);
				mLPButton.setToggleState(true, dontSendNotification);
			}
			if (mBPButton.getToggleState())
			{
				mBPButton.setToggleState(false, dontSendNotification);
				mLPButton.setToggleState(true, dontSendNotification);
			}
		}
		else
		{
			mLPButton.setToggleState(true, dontSendNotification);
		}
	}
	if (b == &mHPButton)
	{
		filterType = FilterType::highpass;
		if (mHPButton.getToggleState())
		{
			if (mLPButton.getToggleState())
			{
				mLPButton.setToggleState(false, dontSendNotification);
				mHPButton.setToggleState(true, dontSendNotification);
			}
			if (mBPButton.getToggleState())
			{
				mBPButton.setToggleState(false, dontSendNotification);
				mHPButton.setToggleState(true, dontSendNotification);
			}
		}
		else
		{
			mHPButton.setToggleState(true, dontSendNotification);
		}
	}
	if (b == &mBPButton)
	{
		filterType = FilterType::bandpass;
		if (mBPButton.getToggleState())
		{
			if (mLPButton.getToggleState())
			{
				mLPButton.setToggleState(false, dontSendNotification);
				mBPButton.setToggleState(true, dontSendNotification);
			}
			if (mHPButton.getToggleState())
			{
				mHPButton.setToggleState(false, dontSendNotification);
				mBPButton.setToggleState(true, dontSendNotification);
			}
		}
		else
		{
			mBPButton.setToggleState(true, dontSendNotification);
		}
	}
}
