#pragma once

#include "CoreMinimal.h"
#include "GenomeHelper.generated.h"

class FGenomes;
/**
 * 
 */
UCLASS()
class GENETICALGORITHMMODULE_API UGenomeHelper : public UObject
{
public:
	GENERATED_BODY()
	FGenomes* RouletteWheelSelection(TArray<FGenomes*> _CurrentGeneration, double _TotalFitnessScore, int32 _PopulationSize);
	FGenomes* Mutate(FGenomes* MutatingGenome, double _MutationRate);

};
