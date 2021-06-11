#include "GeneticAlgorithmController.h"

#include "GeneticAlgorithmFunctionality.h"
#include "GeneticAlgorithmModule.h"
#include "GeneticAlgorithmThread.h"

// Sets default values for this component's properties
AGeneticAlgorithmController::AGeneticAlgorithmController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	// GAFunctions = CreateDefaultSubobject<UGeneticAlgorithmFunctionality>(TEXT("GA Functions"));
	// GAFunctions->AddToRoot();
	// check(GAFunctions != NULL);

	// ...
}

void AGeneticAlgorithmController::CreateFoo()
{
	// GAFunctions = CreateDefaultSubobject<UGeneticAlgorithmFunctionality>(TEXT("GA Functions"));
	GAFunctions = NewObject<UGeneticAlgorithmFunctionality>();
	check(GAFunctions != NULL);
	check(GAFunctions->IsValidLowLevel());
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

// Called when the game starts
void AGeneticAlgorithmController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
		// ...
		UE_LOG(GeneticAlgorithmModule, Display, TEXT("New Enemies found."));

	}
}

TArray<float> AGeneticAlgorithmController::GetNewEnemy() const
{
	UE_LOG(GeneticAlgorithmModule, Display, TEXT("Obtaining New Enemy..."));

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


// Called every frame
void AGeneticAlgorithmController::TickComponent(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ...

	// These two pieces could be moved elsewhere where they're not called all the time in the TickComponent
	// Possibly create a delegate that calls these from the thread? Or a delegate that calls these when needed? e.g. On spawning new enemy/on killing an enemy and init more
	RetrieveCurrentGenerationFromThread(); 
	RetrieveNewEnemiesFromThread(); 
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

