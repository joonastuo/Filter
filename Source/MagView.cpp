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

//==============================================================================

void MagView::paint (Graphics& g)
{
	// Rounded rectangle around the graph
    g.setColour (getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker(.8));
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 10.f);   // draw an outline around the component

	// Graph frequency response
	
	// Needed variables
	float fftLen = fftSize / 2.f;
    float fs = 44100;
    float minX  = log10(20.f * pow(2.f, fftOrder) / fs);
	float scaleX = getWidth() / (log10(fftLen) - minX) ;
	float scaleY = (getHeight() / 2.f) / 20.f;
	float endX = 0.f;
	float endY = 0.f;
	bool startFound = false;

	// Paint markers
	paintMarkers(g, scaleX, minX, fs);
    
	// Path of the frequency response
	Path myPath;
    g.setColour(Colours::white);
    
	for (auto i = 0; i < fftLen; ++i)
	{
		// x and y in the log10 scale
		endX = i == 0 ? 0.f : (log10(i) - minX) * scaleX;
		endY = (getHeight()/2.f - 20.f * log10(mFilteredImpulse[i])*scaleY) ;
		
		
		// Start of the path
		if (startFound == false && (endY > 0.f && endY < getHeight() + 20.f))
		{
			myPath.startNewSubPath(endX, endY);
			startFound = true;
		}
		else
		{
		// Rest of the path that is inside the figure area
		if (endY > 0 && endY < getHeight() + 20.f && startFound == true)
			myPath.lineTo(endX, endY);
		}
	}
	// Draw path with curves
	Path roundedPath = myPath.createPathWithRoundedCorners(5.0f);
	g.strokePath(roundedPath, PathStrokeType(2.f));
}

//==============================================================================

void MagView::paintMarkers(Graphics& g, float scaleX, float minX, float fs)
{
	// Paint horizontal markers
	g.setColour(Colours::white.darker(.8f));
    g.drawLine(0.f, getHeight() / 2.f, getWidth(), getHeight() / 2.f, .2f);
    Line<float> newLine (0.f, getHeight() / 4.f, getWidth(), getHeight() / 4.f);
    float myDash[2] = { 4.f, 4.f };
    g.drawDashedLine(newLine, myDash, 2, .5f);
    Line<float> newLine1 (0.f, getHeight() * .75f, getWidth(), getHeight() * .75f);
    g.drawDashedLine(newLine1, myDash, 2, .5f);

	// Paint vertical markers
    float freq[26] = {30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f,
                  100.f, 200.f, 300.f, 400.f, 500.f, 600.f, 700.f, 800.f, 900.f,
                  1000.f, 2000.f, 3000.f, 4000.f, 5000.f, 6000.f, 7000.f, 8000.f,
                  9000.f, 10000.f};
    for (auto i = 0; i < 26; ++i)
    {
        float markX = (log10(((freq[i]) * pow(2.f, fftOrder)) / fs) - minX) * scaleX;
        g.setColour(Colours::white.darker(.8f));
        float myDash[2] = { 2.f, 2.f };
        if (freq[i] == 10 || freq[i] == 100 || freq[i] == 1000 || freq[i] == 10000)
        {
            g.drawLine(markX, 0.f, markX, getHeight(),.5f);
            if (freq[i] == 1000)
            {
                g.setColour(Colours::white);
                g.drawText("1k", markX - 9.f, getHeight() - 10.f, 20.f, 10.f, Justification::centred);
            }
            if (freq[i] == 100)
            {
                g.setColour(Colours::white);
                g.drawText("100", markX - 9.f, getHeight() - 10.f, 30.f, 10.f, Justification::centred);
            }
            if (freq[i] == 10000)
            {
                g.setColour(Colours::white);
                g.drawText("10k", markX - 9.f, getHeight() - 10.f, 30.f, 10.f, Justification::centred);
            }
        }
        else
            g.drawDashedLine(Line<float>(markX, 0.f, markX, getHeight()), myDash, 2, 1.f);
    }
}

//==============================================================================

void MagView::resized()
{
}

//==============================================================================

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

//==============================================================================

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

//==============================================================================

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
