#pragma once

#include <vector>
#include "utilities.h"

inline 
std::vector<stVertex> 
CreateTorus(float InnerRadius, 
		    float OuterRadius, 
			unsigned int Sides, 
			unsigned int Rings)
{
	
	// A few checks
	DBG_ASSERT_MSG((Sides >= 3), "Sides must be 3 or bigger");
	DBG_ASSERT_MSG((Rings >= 3), "Rings must be 3 or bigger");
	DBG_ASSERT_MSG((InnerRadius >= 0),"InnerRadius can not be negative"
		"(well, you can but than it is no longer a donut)");	
	DBG_ASSERT_MSG((OuterRadius > InnerRadius), "OuterRadius must be bigger than InnerRadius");

	const float centerRadius = InnerRadius * 0.5f + OuterRadius * 0.5f;
	const float rangeRadius = OuterRadius - centerRadius;

	std::vector<stVertex> torus;
	const float stepRing = (360.0f / Rings) * (float)(M_PI/180.0f);
	const float stepSide = (360.0f / Sides) * (float)(M_PI/180.0f);
	float a = 0;
	for(unsigned int i = 0; i < Rings; i++)
	{
		const float curRings[2] = 
		{
			stepRing * (i + 0), 
			stepRing * (i + 1)
		};
		const float ringSins[2] = {sinf(curRings[0]), sinf(curRings[1])};
		const float ringCoss[2] = {cosf(curRings[0]), cosf(curRings[1])};

		for(unsigned int j = 0; j < Sides+1; j++)
		{
			const float curSide = (j % Sides) * stepSide;
			const float sideSin = sinf(curSide);
			const float sideCos = cosf(curSide);

			for(int k = 0; k < 2; k++)
			{
				stVertex vert;	

				vert.x = (centerRadius + rangeRadius * (sideCos)) * ringCoss[k];
				vert.y = (rangeRadius * sideSin);
				vert.z = (centerRadius + rangeRadius * (sideCos)) * ringSins[k];
				vert.rgba = ColorRamp(a / 250.0f);
				a++;
				// If we want textures?
				// instead of 0-->1 we go from 1-->0
				// u = 1.0f - ((1.0f / Rings) * (i+k));	
				// v = ((1.0f / Sides) * (j));

				torus.push_back(vert);
			}
		}
	}

	return torus;
}; // End CreateTorus(..)