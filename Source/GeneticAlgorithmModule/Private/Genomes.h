#pragma once

#include "CoreMinimal.h"

class GENETICALGORITHMMODULE_API FGenomes
{
	public:

	FGenomes() {}
	FGenomes(int32 _NumBits);
	
	void Verify();

	TArray<bool> Bits;
	
	double Fitness = 0;
};
