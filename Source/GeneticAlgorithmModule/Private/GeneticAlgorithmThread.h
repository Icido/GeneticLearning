#pragma once

#include "CoreMinimal.h"
#include "Core/Public/HAL/Runnable.h"

//Forward declarations
class FRunnableThread;
class AGeneticAlgorithmController;
class UGeneticAlgorithmFunctionality;
class UGenomes;
class UGenomeHelper;

class GENETICALGORITHMMODULE_API FGeneticAlgorithmThread : public FRunnable
{
	
public:	
	// Default constructor that takes in the number of Generations to take place initially and the Controller for the thread
	FGeneticAlgorithmThread(UGeneticAlgorithmFunctionality* _GAFunctions, int32 _Generations, AGeneticAlgorithmController *_UGAController);

	// Additional constructor that takes in any previous information that may have been obtained the last time the thread was stopped, as to not start from scratch
	FGeneticAlgorithmThread(UGeneticAlgorithmFunctionality* _GAFunctions, TArray<UGenomes*> _CurrentGenerationGenomes, double _GlobalBestFitness, int32 _Generations, AGeneticAlgorithmController *_UGAController);

	// Helper function to prevent code duplication across both constructors
	void NewThread(UGeneticAlgorithmFunctionality* _GAFunctions, int32 _Generations, AGeneticAlgorithmController* _UGAController);

	bool bStopThread = true;

	/**
	 * @brief Initializes the thread - any thread set-up should be done in here
	 */
	virtual bool Init();

	/**
	 * @breif The "tick" function of the thread - any looping should be done within a while loop inside here
	 */
	virtual uint32 Run();

	/**
	 * @brief Executed when the thread is requested to stop - any cleanup should be done here
	*/
	virtual void Stop();

	/**
	 * @brief Helper function that adds additional Generations to a currently running thread
	 */
	void AddGenerations(int32 _AdditionalGenerations);

private:

	// Total number of Generations to run through 
	int32 Generations;

	// Current Generation number
	int32 GenerationCount;

	AGeneticAlgorithmController *CurrentGAController = nullptr;

	UGeneticAlgorithmFunctionality *GAFunctions = nullptr;

	// TODO: Utilize Genome Helper to convert Genomes to TArray<bool> for output, so not to use Genomes outside of the thread - not currently initialized or in use
	UGenomeHelper *GHelper;
};
