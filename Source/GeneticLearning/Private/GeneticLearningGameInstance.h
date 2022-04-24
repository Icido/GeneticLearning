// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GeneticLearningGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UGeneticLearningGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGeneticLearningGameInstance() : Super() {}

	virtual void Shutdown() override;
	
};
