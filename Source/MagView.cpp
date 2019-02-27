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
	startTimer(100.f);
}

MagView::~MagView()
{
}

void MagView::paint (Graphics& g)
{
    g.setColour (Colours::black);
    g.fillRect (0, 0, getWidth(), getHeight());   // draw an outline around the component

	g.setColour(Colours::white);

	float scaleX = getWidth() / log10(fftSize / 2.f);
	float scaleY = (getHeight() / 2.f) / 20.f;
	float startX = 0.f * scaleX;
	float startY = (getHeight()/2.f - 20.f * log10(mFilteredImpulse[0])*scaleY) ;
	float endX = 0.f;
	float endY = 0.f;
	for (auto i = 1; i < fftSize / 2.f; ++i)
	{
		float endX = log10(i) * scaleX;
		float endY = (getHeight()/2.f - 20.f * log10(mFilteredImpulse[i])*scaleY) ;
		if (endY > -20.f && startY > -20.f && endY < 200 && startY < 200)
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
