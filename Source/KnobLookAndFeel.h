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
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		// Radius of knob
		auto radius = jmin(width / 2, height / 2) - 10.0f;
		// Centre point (centreX, centreY) of knob
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;

		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		// current angle of the slider
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// Paint markers
		paintMarkers(g, centreX, centreY, radius+1.f);

		// Paint knob
		paintKnob(g, centreX, centreY, radius, rotaryStartAngle, rotaryEndAngle);

		// Circle in the centre of the knob
		g.setColour(Colours::white);
		g.fillEllipse(centreX-3.f, centreY-3.f, 6.f, 6.f);

		// Pointer
		Path p;
		auto pointerLength = radius * 1.f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		g.setColour(Colours::white);
		g.fillPath(p);
	}
	void paintMarkers(Graphics & g, float xOrig, float yOrig, float r)
	{
		// Marker colour
		g.setColour(Colours::white);

		// Draw markers with a space of 36 degrees
		for (auto i = -60; i <= 240; ++i)
		{
			float rad = i * (M_PI / 180.f);
			float markerX = xOrig + (r + 5.f) * cos(rad);
			float markerY = yOrig + (r + 5.f) * -sin(rad);

			// Draw ticks
			if (abs(i) % 36 == 18)
				g.fillEllipse(markerX - 1.4f, markerY - 1.4f, 2.8f, 2.8f);
		}
	}
	void paintKnob(Graphics& g, float centreX, float centreY, float radius, float startAngle, float endAngle)
	{
		// Path of the knob
		Path p;
		bool pathStart = true;
		float firstX = 0.0f;
		float firstY = 0.0f;
		for (auto i = -55; i <= 235; ++i)
		{
			// Angle from degrees to radians
			float radians = i * (M_PI / 180.f);
			// Marker point (pointX, pointY)
			float pointX = centreX + radius * cos(radians);
			float pointY = centreY + radius * -sin(radians);

			// Start path if first point
			if (pathStart)
			{
				p.startNewSubPath(pointX, pointY);
				// Save start point so path can be ended there
				firstX = pointX;
				firstY = pointY;
				pathStart = false;
			}
			// Add point if not start
			else
			{
				p.lineTo(pointX, pointY);
			}

		}
		// Path to origin
		p.lineTo(centreX, centreY);
		// Path to start point
		p.lineTo(firstX, firstY);
		// Make path rounded
		Path roundedPath = p.createPathWithRoundedCorners(10.0f);
		// Set colour to darder version of background colour
		Colour myColour = findColour(0x1005700);
		myColour = myColour.darker(.8f);
		g.setColour(myColour);
		// fill knob background
		g.fillPath(roundedPath);
		g.setColour(Colours::white);
		// g.strokePath(roundedPath, PathStrokeType(1.f));
	}
};
