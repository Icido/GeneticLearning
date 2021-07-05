#include "GeneticAlgorithmController.h"

#include "GeneticAlgorithmFunctionality.h"
#include "GeneticAlgorithmModule.h"
#include "GeneticAlgorithmResults.h"
#include "GeneticAlgorithmThread.h"

// Sets default values for this component's properties
AGeneticAlgorithmController::AGeneticAlgorithmController()
{
	// ...
}

void AGeneticAlgorithmController::Setup()
{
	GAFunctions = NewObject<UGeneticAlgorithmFunctionality>();
	GAResults = NewObject<AGeneticAlgorithmResults>();
}

void AGeneticAlgorithmController::InitGenerations(int32 _NumGenerations)
{
	check(GAFunctions->IsValidLowLevel());
	check(GAFunctions != NULL);

	if(_NumGenerations > 0)
	{
		// If there's a thread active currently, the number of generations will be tacked on
		if(CurrentRunningThread && GAThread)
		{
			CurrentRunningThread->Suspend(true);

			if(GAThread->bStopThread == false)
			{
				GAThread->AddGenerations(_NumGenerations);
				CurrentRunningThread->Suspend(false);
				
				return;
			}

			CurrentRunningThread->Suspend(false);
		}
		
		check(GAFunctions != NULL);

		GAResults->RetrieveCurrentGenerationFromThread();

		GAThread = new FGeneticAlgorithmThread(GAFunctions, _NumGenerations, GAResults);

		CurrentRunningThread = FRunnableThread::Create(GAThread, TEXT("Genetic Algorithm Thread"));
	}
	else
	{
		UE_LOG(GeneticAlgorithmModule, Warning, TEXT("Number of generations must be greater than 0. Could not parse %d"), _NumGenerations);
	}
}

TArray<float> AGeneticAlgorithmController::GetNewEnemy() const
{
	return GAResults->EnemySelection();
}

// TODO: Fix thread shutdown to prevent crash
void AGeneticAlgorithmController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(CurrentRunningThread && GAThread)
	{
		CurrentRunningThread->Suspend(true);
		GAThread->bStopThread = true;
		CurrentRunningThread->Suspend(false);
		CurrentRunningThread->Kill(false);
		CurrentRunningThread->WaitForCompletion();

		delete GAThread;
	}
}

