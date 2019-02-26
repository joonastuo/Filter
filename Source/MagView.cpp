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
MagView::MagView(AudioProcessorValueTreeState& vt) : mParameters(vt), mFFT(10)
{
	setSize(200, 100);
	startTimer(20.f);
}

MagView::~MagView()
{
}

void MagView::paint (Graphics& g)
{
    g.setColour (Colour(40, 190, 127).darker(1.f));
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 10.f);   // draw an outline around the component

	float startX = 0.f;
	float startY = 20.f * log10(mFilteredImpulse[0]);
	float endX = 0.f;
	float endY = 0.f;
	float scale = getWidth() / (fftSize / 2.f);
	for (auto i = 1; i < fftSize / 2.f; ++i)
	{
		float endX = i*scale;
		float endY = 20.f * log10(abs(mFilteredImpulse[i]));
		if (endY > 0 && startY > 0 && endY < 100 && startY < 100)
			g.drawLine(startX, startY, endX, endY, 2.f);
		startX = endX;
		startY = endY;
	}

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
	if (fc != mOldFc || res != mOldRes)
	{
		updateFilter();
		calcMagResponse();
		repaint();
	}
	mOldFc = fc;
	mOldRes = res;
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
