// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneticLearningGameInstance.h"
#include "GeneticAlgorithmController.h"

void UGeneticLearningGameInstance::Shutdown()
{
	bool gThreadStopped = false;

	while(!gThreadStopped)
	{
		//check to update gThreadStopped
		gThreadStopped = true;
		GLog->Log("Genetic Thread still running... (GI)");
	}
}
