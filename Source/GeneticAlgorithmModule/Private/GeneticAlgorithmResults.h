#pragma once
#include "GeneticAlgorithmController.h"
#include "GeneticAlgorithmResults.generated.h"

UCLASS()
class GENETICALGORITHMMODULE_API AGeneticAlgorithmResults : public AGeneticAlgorithmController
{
	GENERATED_BODY()
public:
	AGeneticAlgorithmResults();

	/**
	* @brief Retrieves the latest generation of genomes, as well as the best fitness score so far, from the queue holding the data
	*/
	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void RetrieveCurrentGenerationFromThread();

	/**
	* @brief Retrieves the current queue of genomes that represents the fittest enemies. These enemies are already sorted by fitness, with the last being the fittest
	*/
	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void RetrieveNewEnemiesFromThread();

	bool IsCurrentGenerationEmpty() const;

	TArray<UGenomes*> RetrieveCurrentGenerationFromResults() const;

	double RetrieveGlobalBestFitnessFromResults() const;

	TArray<float> EnemySelection();

	// The queues used to obtain data from the thread 
	TQueue<TArray<UGenomes*>> ThreadGenomeGenerationQueue;
	TQueue<TArray<TArray<float>>> ThreadNewEnemyQueue;
	TQueue<double> ThreadGlobalBestFitnessScoreQueue;

private:

	
	// The best fitness score of all generations so far - used to ensure that newer generations have a goal to aim toward before being considered "fitter"
	UPROPERTY()
	double GlobalBestFitness = 0;

	// Storage for the obtained Genomes
	TArray<UGenomes*> CurrentGeneration;

	// Storage for the obtained converted Genomes
	TArray<TArray<float>> CurrentUsableEnemies;
};
