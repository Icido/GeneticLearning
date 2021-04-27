#include "GeneticAlgorithmController.h"

#include "GeneticAlgorithmModule.h"
#include "GeneticAlgorithmThread.h"

// Sets default values for this component's properties
UGeneticAlgorithmController::UGeneticAlgorithmController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGeneticAlgorithmController::InitGenerations(int32 _NumGenerations)
{
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
		
		if(CurrentGeneration.Num() == 0)
		{
			GAThread = new FGeneticAlgorithmThread(_NumGenerations, this);
		}
		else
		{
			GAThread = new FGeneticAlgorithmThread(CurrentGeneration, GlobalBestFitness, _NumGenerations, this);
		}

		CurrentRunningThread = FRunnableThread::Create(GAThread, TEXT("Genetic Algorithm Thread"));
	}
	else
	{
		UE_LOG(GeneticAlgorithmModule, Warning, TEXT("Number of generations must be greater than 0. Could not parse %d"), _NumGenerations);
	}
}

// Called when the game starts
void UGeneticAlgorithmController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGeneticAlgorithmController::RetrieveCurrentGenerationFromThread()
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

void UGeneticAlgorithmController::RetrieveNewEnemiesFromThread()
{
	if(!ThreadNewEnemyQueue.IsEmpty() && ThreadNewEnemyQueue.Dequeue(CurrentUsableEnemies))
	{
		// ...
	}
}

TArray<float> UGeneticAlgorithmController::GetNewEnemy() const
{
	if(CurrentUsableEnemies.Num() <= 0)
		return TArray<float> {};

	const int32 EnemyIndex = FMath::Max3(FMath::RandRange(0, CurrentUsableEnemies.Num() - 1),
	                                     FMath::RandRange(0, CurrentUsableEnemies.Num() - 1),
	                                     FMath::RandRange(0, CurrentUsableEnemies.Num() - 1));
	
	return CurrentUsableEnemies[EnemyIndex];
}


// Called every frame
void UGeneticAlgorithmController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// These two pieces could be moved elsewhere where they're not called all the time in the TickComponent
	// Possibly create a delegate that calls these from the thread? Or a delegate that calls these when needed? e.g. On spawning new enemy/on killing an enemy and init more
	RetrieveCurrentGenerationFromThread(); 
	RetrieveNewEnemiesFromThread(); 
}

void UGeneticAlgorithmController::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

