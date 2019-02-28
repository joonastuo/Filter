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
		// fill
		g.setColour(Colours::black);
		g.fillEllipse(rx, ry, rw, rw);
		paintMarkers(g, centreX, centreY, radius+1.f);
		// outline
		Path p;
		auto pointerLength = radius * 0.33f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		// pointer
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
			float endX = xOrig + (r + 3.f) * cos(rad);
			float endY = yOrig + (r + 3.f) * -sin(rad);

			// Draw ticks
			if (abs(i) % 36 == 18)
				g.drawLine(startX, startY, endX, endY, 2.f);
		}
	}
};
