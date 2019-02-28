/*
  ==============================================================================

    MagView.cpp
    Created: 20 Feb 2019 4:48:07pm
    Author:  Joonas

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MagView.h"

//==============================================================================
MagView::MagView(AudioProcessorValueTreeState& vt) : mParameters(vt), mFFT(fftOrder)
{
	setSize(200, 80);
	startTimer(10.f);
}

MagView::~MagView()
{
}

void MagView::paint (Graphics& g)
{
	// Rounded rectangle around the graph
    g.setColour (getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker(.8));
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 10.f);   // draw an outline around the component


	g.setColour(Colours::white.darker(.8f));
	float myDash[2] = { 4.f, 4.f };
	g.drawDashedLine(Line<float>(0.f, getHeight() / 2.f, getWidth(), getHeight() / 2.f), myDash, 2, 1.f);
	// Graph frequency response
	g.setColour(Colours::white);
	// Needed variables
	float fftLen = fftSize / 2.f;
	float scaleX = getWidth() / log10(fftLen);
	float scaleY = (getHeight() / 2.f) / 20.f;
	float endX = 0.f;
	float endY = 0.f;
	bool startFound = false;

	// Path of the frequency response
	Path myPath;

	for (auto i = 0; i < fftLen; ++i)
	{
		// x and y in the log10 scale
		endX = i == 0 ? 0.f : log10(i) * scaleX;
		endY = (getHeight()/2.f - 20.f * log10(mFilteredImpulse[i])*scaleY) ;
		
		if (i == round(pow(2.f, 3.5f)) || i == round(pow(2.f, 7.f)) || i == round(pow(2.f, 10.5f)))
		{
			g.setColour(Colours::white.darker(.8f));
			float myDash[2] = { 4.f, 4.f };
			g.drawDashedLine(Line<float>(endX, 0.f, endX, getHeight()), myDash, 2, 1.f);
			g.setColour(Colours::white);
		}

		// Start of the path
		if (startFound == false && (endY > 0.f && endY < getHeight() + 20.f))
		{
			myPath.startNewSubPath(endX, endY);
			startFound = true;
		}
		else
		{
		// Rest of the path that is inside the figure area
		if (endY > 0 && endY < getHeight() && startFound == true)
			myPath.lineTo(endX, endY);
		}
	}
	// Draw path with curves
	Path roundedPath = myPath.createPathWithRoundedCorners(5.0f);
	g.strokePath(roundedPath, PathStrokeType(2.f));
}

void MagView::resized()
{
}

void MagView::updateFilter()
{
	float fc = *mParameters.getRawParameterValue("fc");
	float res = *mParameters.getRawParameterValue("res");
	float fs = mParameters.state[IDs::fs];
	float selectFilter = *mParameters.getRawParameterValue("selectFilter");

	mStateVariableFilter.parameters->setCutOffFrequency(fs, fc, res);

	if (selectFilter == 0)
	{
		mStateVariableFilter.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
	}
	else if (selectFilter == 1)
	{
		mStateVariableFilter.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
	}
	else if (selectFilter == 2)
	{
		mStateVariableFilter.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
	}
}

void MagView::timerCallback()
{
	float fc = *mParameters.getRawParameterValue("fc");
	float res = *mParameters.getRawParameterValue("res");
	float selectFilter = *mParameters.getRawParameterValue("selectFilter");
	
	if (fc != mOldFc || res != mOldRes || selectFilter != mOldSelect)
	{
		updateFilter();
		calcMagResponse();
		repaint();
	}
	mOldFc = fc;
	mOldRes = res;
	mOldSelect = selectFilter;
}

void MagView::calcMagResponse()
{
	// Create impulse
	zeromem(mImpulse, sizeof(mImpulse));
	mImpulse[0] = 1;
	zeromem(mFilteredImpulse, sizeof(mFilteredImpulse));

	for (auto i = 0; i < fftSize; ++i)
	{
		mFilteredImpulse[i] = mStateVariableFilter.processSample(mImpulse[i]);
	}
	mFFT.performFrequencyOnlyForwardTransform(mFilteredImpulse);
}
