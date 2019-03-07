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
	// Mag view size
	//setSize(getHeight() / 1.8f, getWidth());
	// Start update timer
	startTimer(10.f);
}

MagView::~MagView()
{

}

//==============================================================================

void MagView::paint (Graphics& g)
{
	// Rounded rectangle around the graph with same but darker colour as the background
    g.setColour (getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker(.8));
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 10.f);   // draw an outline around the component

	// Graph frequency response====================================
	
	// Needed variables
	// Length of usable values in fft (fs / 2)
	float fftLen = fftSize / 2.f;
	// Sampling frequency
    float fs = mParameters.state[IDs::fs];
	// Start point in x-axis (20 Hz)
    float minX  = log10(20.f * pow(2.f, fftOrder) / fs);
	// Scale x-axis so that start is at 20 Hz and end at fs / 2
	float scaleX = getWidth() / (log10(fftLen) - minX) ;
	// Scale y-axis from -12 dB to 12 dB
	float maxY = 12.f;
	float scaleY = (getHeight() / 2.f) / maxY;
	float endX = 0.f;
	float endY = 0.f;
	bool startFound = false;

	// Paint markers
	paintMarkers(g, scaleX, minX, fs);
    
	// Path of the frequency response
	Path myPath;
	// Path colour
    g.setColour(Colours::white);
    
	for (auto i = 0; i < fftLen; ++i)
	{
		// X scaled for window size and log10 scale
		endX = i == 0 ? 0.f : (log10(i) - minX) * scaleX;
		endY = (getHeight() / 2.f - maxY * log10(mFilteredImpulse[i]) * scaleY) ;
		
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
	// Horizontal marker colour
	// Dash size for dashed lines
    float myDash[2] = { 2.f, 2.f };
	// Colour for lines (horizontal and vertical)
	g.setColour(Colours::white.darker(.8f));

	for (auto i = 1; i < 8; ++i)
	{
		Line<float> horizontalLine (0.f, getHeight() * (i / 8.f), getWidth(), getHeight() * (i / 8.f));
		if (i == 4)
			g.drawLine(horizontalLine, .5f);
		else
			g.drawDashedLine(horizontalLine, myDash, 2, .5f);
	}

	// List of vertical marker x values
    float freq[26] = {30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f,
                  100.f, 200.f, 300.f, 400.f, 500.f, 600.f, 700.f, 800.f, 900.f,
                  1000.f, 2000.f, 3000.f, 4000.f, 5000.f, 6000.f, 7000.f, 8000.f,
                  9000.f, 10000.f};

	// Paint vertical parkers
    for (auto i = 0; i < 26; ++i)
    {
		// X scaled for window size and log10 scale
        float markX = (log10(((freq[i]) * pow(2.f, fftOrder)) / fs) - minX) * scaleX;

		// Draw solid lines to 100, 1000 and 10 000
        if (freq[i] == 100 || freq[i] == 1000 || freq[i] == 10000)
        {
			g.drawLine(markX, 0.f, markX, getHeight(),.5f);
			g.setColour(Colours::white);
			// Text indicating frequencies
            if (freq[i] == 100)
                g.drawText("100", markX - 9.f, getHeight() - 10.f, 30.f, 10.f, Justification::centred);
            else if (freq[i] == 1000)
                g.drawText("1k", markX - 9.f, getHeight() - 10.f, 20.f, 10.f, Justification::centred);
            else if (freq[i] == 10000)
                g.drawText("10k", markX - 9.f, getHeight() - 10.f, 30.f, 10.f, Justification::centred);

			g.setColour(Colours::white.darker(.8f));
        }
		else
		{
			// Paint line
            g.drawDashedLine(Line<float>(markX, 0.f, markX, getHeight()), myDash, 2, 1.f);
		}
    }
}

//==============================================================================

void MagView::resized()
{
}

//==============================================================================

void MagView::updateFilter()
{
	// Filter parameters from plugin ValueTree
	float fc = *mParameters.getRawParameterValue("fc");
	float res = *mParameters.getRawParameterValue("res");
	float fs = mParameters.state[IDs::fs];
	float filterType = *mParameters.getRawParameterValue("selectFilter");

	// Set filter fs, fc and res
	mStateVariableFilter.parameters->setCutOffFrequency(fs, fc, res);

	// Low pass filter
	if (filterType == 0)
	{
		mStateVariableFilter.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
	}
	// High pass filter
	else if (filterType == 1)
	{
		mStateVariableFilter.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
	}
	// Band pass filter
	else if (filterType == 2)
	{
		mStateVariableFilter.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
	}
}

//==============================================================================

void MagView::timerCallback()
{
	// Repaint filter magnitude response if filter parameters have changed.
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

	// Filter impulse
	for (auto i = 0; i < fftSize; ++i)
	{
		mFilteredImpulse[i] = mStateVariableFilter.processSample(mImpulse[i]);
	}
	// Perform fft
	mFFT.performFrequencyOnlyForwardTransform(mFilteredImpulse);
}
