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
*/
class KnobLookAndFeel : public LookAndFeel_V4
{
public:
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto radius = jmin(width / 2, height / 2) - 10.0f;
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// Paint markers
		paintMarkers(g, centreX, centreY, radius+1.f);

		// Knob
		paintKnob(g, centreX, centreY, radius, rotaryStartAngle, rotaryEndAngle);
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
		g.setColour(Colours::white);
		for (auto i = -60; i <= 240; ++i)
		{
			float rad = i * (M_PI / 180.f);
			float startX = xOrig + r * cos(rad);
			float startY = yOrig + r * -sin(rad);
			float endX = xOrig + (r + 5.f) * cos(rad);
			float endY = yOrig + (r + 5.f) * -sin(rad);

			// Draw ticks
			if (abs(i) % 36 == 18)
				g.fillEllipse(endX - 1.4f, endY - 1.4f, 2.8f, 2.8f);
				//g.drawLine(startX, startY, endX, endY, 2.f);
		}
	}
	void paintKnob(Graphics& g, float xOrig, float yOrig, float r, float startAngle, float endAngle)
	{
		g.setColour(Colours::aliceblue);
		Path p;
		bool pathStart = true;
		float firstX = 0.0f;
		float firstY = 0.0f;
		for (auto i = -55; i <= 235; ++i)
		{
			float rad = i * (M_PI / 180.f);
			float startX = xOrig + r * cos(rad);
			float startY = yOrig + r * -sin(rad);

			if (pathStart)
			{
				p.startNewSubPath(startX, startY);
				firstX = startX;
				firstY = startY;
				pathStart = false;
			}
			else
			{
				p.lineTo(startX, startY);
			}

		}
		p.lineTo(xOrig, yOrig + 8.f);
		p.lineTo(firstX, firstY);
		Path roundedPath = p.createPathWithRoundedCorners(20.0f);
		Colour myColour = findColour(0x1005700);
		myColour = myColour.darker(.2f);
		g.setColour(myColour);
		g.fillPath(roundedPath);
		g.setColour(Colours::white);
		g.strokePath(roundedPath, PathStrokeType(1.f));
	}
};
