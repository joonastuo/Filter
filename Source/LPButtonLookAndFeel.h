/*
  ==============================================================================

    LPButtonLookAndFeel.h
    Created: 2 Mar 2019 3:31:07pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class LPButtonLookAndFeel : public LookAndFeel_V4
{
public:
	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override
	{
		auto buttonArea = button.getLocalBounds();
		float width = button.getWidth();
		float height = button.getHeight();

		Path buttonPath;
		buttonPath.startNewSubPath(0.f, height * .2f);
		buttonPath.lineTo(0.f, height * .8f);
		buttonPath.lineTo(width * .2f, height);
		buttonPath.lineTo(width * .8f, height);
		buttonPath.lineTo(width, height * .8f);
		buttonPath.lineTo(width, height * .2f);
		buttonPath.lineTo(width * .8f, 0.f);
		buttonPath.lineTo(width * .2f, 0.f);
		buttonPath.lineTo(0.f, height * .2f);
		buttonPath.lineTo(0.f, height * .8f);
		Path roundedButtonPath = buttonPath.createPathWithRoundedCorners(10.0f);
		g.setColour(backgroundColour);
		g.fillPath(roundedButtonPath);
		g.setColour(Colours::white);

		if (button.getToggleState())
			g.setColour(Colours::white);
		else
			g.setColour(Colours::white.darker(.4f));

		g.strokePath(roundedButtonPath, PathStrokeType(1.f));

		// Graph of LP filter
		Path p;
		p.startNewSubPath(0.f, height / 2.f);
		p.lineTo(width / 2.f, height / 2.f);
		p.lineTo(width * .75f, height);
		Path roundedPath = p.createPathWithRoundedCorners(20.0f);
		g.strokePath(roundedPath, PathStrokeType(1.f));
	}
};