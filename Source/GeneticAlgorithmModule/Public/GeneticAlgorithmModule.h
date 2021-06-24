// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(GeneticAlgorithmModule, All, All);

class FGeneticAlgorithmModule : public IModuleInterface
{
	public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};

// TODO: Send current player data to be fed into the GA - Figure out what information would be relevant
// TODO: Clean up any error handling/unused functionality for smoother operation
// TODO: Rework public functions for blueprint accessibility
// TODO: Implement public functions to modify the algorithm for their needs (e.g. change test, num bits, generation sizes)
// TODO: Comment and document any errant functionality to be as clear as possible for future use