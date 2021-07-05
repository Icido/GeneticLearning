#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAL/RunnableThread.h"
#include "GeneticAlgorithmController.generated.h"

// Forward declarations
class UGenomes;
class UGeneticAlgorithmFunctionality;
class FGeneticAlgorithmThread;
class AGeneticAlgorithmResults;

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
	

	/**
	* @returns Retrieve a new enemy's stats
	*/
	TArray<float> GetNewEnemy() const;	

protected:

	FGeneticAlgorithmThread *GAThread = nullptr;
	
	FRunnableThread *CurrentRunningThread = nullptr;

	UPROPERTY(VisibleAnywhere)
	UGeneticAlgorithmFunctionality *GAFunctions = nullptr;

	UPROPERTY(VisibleAnywhere)
	AGeneticAlgorithmResults *GAResults = nullptr;
	
private:	

	// Called for the sake of thread safety
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Total number of Generations that has been calculated
	UPROPERTY()
	int32 TotalGenerations = 0;
};
