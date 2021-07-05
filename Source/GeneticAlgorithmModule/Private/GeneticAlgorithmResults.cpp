#include "GeneticAlgorithmResults.h"
#include "GeneticAlgorithmModule.h"

AGeneticAlgorithmResults::AGeneticAlgorithmResults()
{
}

void AGeneticAlgorithmResults::RetrieveCurrentGenerationFromThread()
{
	if(!ThreadGenomeGenerationQueue.IsEmpty() && ThreadGenomeGenerationQueue.Dequeue(CurrentGeneration))
	{
		// UE_LOG(GeneticAlgorithmModule, Display, TEXT("Retrieved last generation."));
	}

	if(!ThreadGlobalBestFitnessScoreQueue.IsEmpty() && ThreadGlobalBestFitnessScoreQueue.Dequeue(GlobalBestFitness))
	{
		UE_LOG(GeneticAlgorithmModule, Display, TEXT("Last generation's current record of Fitness: %f"), GlobalBestFitness);
	}
}

void AGeneticAlgorithmResults::RetrieveNewEnemiesFromThread()
{
	if(!ThreadNewEnemyQueue.IsEmpty() && ThreadNewEnemyQueue.Dequeue(CurrentUsableEnemies))
	{
		// UE_LOG(GeneticAlgorithmModule, Display, TEXT("Dequeued CurrentUsableEnemies."));
	}
}

bool AGeneticAlgorithmResults::IsCurrentGenerationEmpty() const
{
	return CurrentGeneration.Num() == 0;
}

TArray<UGenomes*> AGeneticAlgorithmResults::RetrieveCurrentGenerationFromResults() const
{
	return CurrentGeneration;
}

double AGeneticAlgorithmResults::RetrieveGlobalBestFitnessFromResults() const
{
	return GlobalBestFitness;
}

TArray<float> AGeneticAlgorithmResults::EnemySelection()
{
	RetrieveNewEnemiesFromThread();
	
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
