#include "GeneticAlgorithmController.h"

#include "GeneticAlgorithmFunctionality.h"
#include "GeneticAlgorithmModule.h"
#include "GeneticAlgorithmThread.h"

// Sets default values for this component's properties
AGeneticAlgorithmController::AGeneticAlgorithmController()
{
	// ...
}

void AGeneticAlgorithmController::Setup()
{
	GAFunctions = NewObject<UGeneticAlgorithmFunctionality>();
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

				UE_LOG(GeneticAlgorithmModule, Display, TEXT("Added %d additional generations."), _NumGenerations);
				
				return;
			}

			CurrentRunningThread->Suspend(false);
		}
		
		check(GAFunctions != NULL);

		//TODO: Check if content within threadqueues remain after thread is finished - if so, this would need to be called before thread closes as to hold locally
		RetrieveCurrentGenerationFromThread();

		if(CurrentGeneration.Num() == 0)
		{
			GAThread = new FGeneticAlgorithmThread(GAFunctions, _NumGenerations, this);
		}
		else
		{
			GAThread = new FGeneticAlgorithmThread(GAFunctions, CurrentGeneration, GlobalBestFitness, _NumGenerations, this);
		}

		CurrentRunningThread = FRunnableThread::Create(GAThread, TEXT("Genetic Algorithm Thread"));
	}
	else
	{
		UE_LOG(GeneticAlgorithmModule, Warning, TEXT("Number of generations must be greater than 0. Could not parse %d"), _NumGenerations);
	}
}

TArray<float> AGeneticAlgorithmController::GetNewEnemy()
{
	RetrieveNewEnemiesFromThread();

	return EnemySelect();
}

TArray<float> AGeneticAlgorithmController::EnemySelect() const
{
	if(CurrentUsableEnemies.Num() <= 0)
	{
		UE_LOG(GeneticAlgorithmModule, Warning, TEXT("No New Enemies available."));

		return TArray<float> {};
	}

	const int32 EnemyIndex = FMath::Max3(FMath::RandRange(0, CurrentUsableEnemies.Num() - 1),
										FMath::RandRange(0, CurrentUsableEnemies.Num() - 1),
										FMath::RandRange(0, CurrentUsableEnemies.Num() - 1));
	
	return CurrentUsableEnemies[EnemyIndex];
}

void AGeneticAlgorithmController::RetrieveCurrentGenerationFromThread()
{
	if(!ThreadGenomeGenerationQueue.IsEmpty() && ThreadGenomeGenerationQueue.Dequeue(CurrentGeneration))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Retrieved last generation."));
	}

	if(!ThreadGlobalBestFitnessScoreQueue.IsEmpty() && ThreadGlobalBestFitnessScoreQueue.Dequeue(GlobalBestFitness))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Last generation's current record of Fitness: %f"), GlobalBestFitness));
	}
}

void AGeneticAlgorithmController::RetrieveNewEnemiesFromThread()
{
	if(!ThreadNewEnemyQueue.IsEmpty() && ThreadNewEnemyQueue.Dequeue(CurrentUsableEnemies))
	{
		//UE_LOG(GeneticAlgorithmModule, Display, TEXT("Dequeued CurrentUsableEnemies."));
	}
}

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

