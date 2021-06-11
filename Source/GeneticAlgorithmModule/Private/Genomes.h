#pragma once

#include "CoreMinimal.h"
#include "Genomes.generated.h"

UCLASS()
class GENETICALGORITHMMODULE_API UGenomes : public UObject
{
	public:
	GENERATED_BODY()
	
	UFUNCTION()
	void InitGenome(int32 _NumBits);

	UFUNCTION()
	void AssignBits(TArray<bool> _NewBits);
	void AssignSingleBit(int32 _BitIndex, bool _NewBit);

	UFUNCTION()
	void Verify();

	UFUNCTION()
	bool ClearGenome();

	UPROPERTY()
	TArray<bool> Bits;

	UPROPERTY()
	double Fitness = 0;
};
