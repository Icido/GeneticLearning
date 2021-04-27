#pragma once

#include "CoreMinimal.h"
#include "SmartEnemyTest.generated.h"

/**
 * 
 */
UCLASS()
class GENETICALGORITHMMODULE_API USmartEnemyTest : public UObject
{
public:
	GENERATED_BODY()
	double FitnessTest(TArray<float> _SmartEnemyAttributes) const;
};
