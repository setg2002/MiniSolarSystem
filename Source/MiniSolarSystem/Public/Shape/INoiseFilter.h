// Copyright Soren Gilbertson

#pragma once

/**
 * 
 */

class INoiseFilter
{
public:
	INoiseFilter();
	virtual ~INoiseFilter();
	
	virtual float Evaluate(FVector point);
};
