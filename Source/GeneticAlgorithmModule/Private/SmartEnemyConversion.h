#pragma once

#include "CoreMinimal.h"
#include "SmartEnemyConversion.generated.h"

class UGenomes;
/**
 * 
 */
UCLASS()
class GENETICALGORITHMMODULE_API USmartEnemyConversion : public UObject
{
public:
	GENERATED_BODY()
	TArray<int32> DecodeGenome(UGenomes* _Genomes, int32 _GeneLength);
	int32 GeneToInt32(TArray<bool> _Gene);
	TArray<float> DecodedMutations(TArray<int32> _Mutations, int32 _GeneLength);
	TArray<float> GenomeToEnemy(UGenomes* _Genomes, int32 _GeneLength);
};
