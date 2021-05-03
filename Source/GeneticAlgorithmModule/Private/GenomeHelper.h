#pragma once

#include "CoreMinimal.h"

#include "GenomeHelper.generated.h"

class UGenomes;
/**
 * 
 */
UCLASS()
class GENETICALGORITHMMODULE_API UGenomeHelper : public UObject
{
public:
	GENERATED_BODY()
	UGenomeHelper();

	UFUNCTION()
	TArray<bool> RouletteWheelSelection(TArray<UGenomes*> _CurrentGeneration, double _TotalFitnessScore, int32 _PopulationSize);

	UFUNCTION()
	void Mutate(UGenomes* MutatingGenome, double _MutationRate);

	//FGenomes* MutatedGenome;
	UPROPERTY()
	UGenomes* DefaultGenome;
	
};
