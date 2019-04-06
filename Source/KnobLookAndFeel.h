/*
  ==============================================================================

    MyKnobs.h
    Created: 26 Feb 2019 8:30:04pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
/*
LookAndFeel of filter knobs. Use as the LookAndFeel of slider component.
*/
class KnobLookAndFeel : public LookAndFeel_V4
{
public:
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider&) override
	{
		// Radius of knob
		auto radius = jmin(width / 2, height / 2) - 5.0f;
		// Centre point (centreX, centreY) of knob
		auto centreX = x + width * 0.5f;
		auto centreY = y + radius + 12.f;

		// current angle of the slider
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// Draw path of the slider backgound (in darker background colour)
		Path backgroundArc;
		backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
		Colour myColour = findColour(0x1005700);
		myColour = myColour.darker(.8f);
		g.setColour(myColour);
		g.strokePath(backgroundArc, PathStrokeType(3.f, PathStrokeType::curved, PathStrokeType::rounded));
		// Draw path of slider foreground (in white)
		Path foregroundArc;
		foregroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
		g.setColour(Colours::white);
		g.strokePath(foregroundArc, PathStrokeType(3.f, PathStrokeType::curved, PathStrokeType::rounded));

		// Pointer
		Path p;
		auto pointerLength = radius * 1.f;
		auto pointerThickness = 3.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		g.fillPath(p);
	}

	// Slider textbox
	void drawLabel(Graphics& g, Label& label) override
	{
		g.setColour(Colours::white);
		String text = label.getText();
		int width = label.getWidth();
		int height = label.getHeight();
		g.setFont(Font(static_cast<float>(height - 2), Font::plain));
		g.drawFittedText(text, 0, 0, width, height, Justification::centred, 1);
	}
};
