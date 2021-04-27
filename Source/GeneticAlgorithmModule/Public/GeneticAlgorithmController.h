#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAL/RunnableThread.h"
#include "GeneticAlgorithmController.generated.h"

// Forward declarations
class FGenomes;

UCLASS()
class GENETICALGORITHMMODULE_API UGeneticAlgorithmController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGeneticAlgorithmController();

	/**
	 * @brief Initializes a thread (if not already running) and adds to the number of Generations that are completed
	 * @param _NumGenerations Number of additional Generations that the algorithm executes
	 */
	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void InitGenerations(int32 _NumGenerations);

	/**
	* @returns Retrieve a new enemy's stats
	*/
	TArray<float> GetNewEnemy() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called for the sake of thread safety
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Total number of Generations that has been calculated
	UPROPERTY()
	int32 TotalGenerations = 0;

	// The best fitness score of all generations so far - used to ensure that newer generations have a goal to aim toward before being considered "fitter"
	UPROPERTY()
	double GlobalBestFitness = 0;

	//TODO: Change TQueue to something readable for use in creating Smart Enemies (possibly, works currently with SEConvert being used in SE)
	// The queues used to obtain data from the thread 
	TQueue<TArray<FGenomes*>> ThreadGenomeGenerationQueue;
	TQueue<TArray<TArray<float>>> ThreadNewEnemyQueue;
	TQueue<double> ThreadGlobalBestFitnessScoreQueue;

	// Storage for the obtained Genomes
	TArray<FGenomes*> CurrentGeneration;

	// Storage for the obtained converted Genomes
	TArray<TArray<float>> CurrentUsableEnemies;
};
