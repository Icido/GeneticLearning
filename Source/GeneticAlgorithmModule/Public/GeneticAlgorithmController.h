#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAL/RunnableThread.h"
#include "GeneticAlgorithmController.generated.h"

// Forward declarations
class UGenomes;
class UGeneticAlgorithmFunctionality;

UCLASS()
class GENETICALGORITHMMODULE_API AGeneticAlgorithmController : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AGeneticAlgorithmController();

	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void Setup();
	
	/**
	* @brief Initializes a thread (if not already running) and adds to the number of Generations that are completed
	* @param _NumGenerations Number of additional Generations that the algorithm executes
	*/
	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void InitGenerations(int32 _NumGenerations);
	
	UPROPERTY(VisibleAnywhere)
	UGeneticAlgorithmFunctionality *GAFunctions = nullptr;

	/**
	* @returns Retrieve a new enemy's stats
	*/
	TArray<float> GetNewEnemy();


protected:

	TArray<float> EnemySelect() const;
	
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

	class FGeneticAlgorithmThread *GAThread = nullptr;
	
	FRunnableThread *CurrentRunningThread = nullptr;

public:	

	// Called for the sake of thread safety
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Total number of Generations that has been calculated
	UPROPERTY()
	int32 TotalGenerations = 0;

	// The best fitness score of all generations so far - used to ensure that newer generations have a goal to aim toward before being considered "fitter"
	UPROPERTY()
	double GlobalBestFitness = 0;

	// The queues used to obtain data from the thread 
	TQueue<TArray<UGenomes*>> ThreadGenomeGenerationQueue;
	TQueue<TArray<TArray<float>>> ThreadNewEnemyQueue;
	TQueue<double> ThreadGlobalBestFitnessScoreQueue;

	// Storage for the obtained Genomes
	TArray<UGenomes*> CurrentGeneration;

	// Storage for the obtained converted Genomes
	TArray<TArray<float>> CurrentUsableEnemies;
};
