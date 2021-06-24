#include "GeneticAlgorithmThread.h"
#include "GeneticAlgorithmController.h"
#include "GeneticAlgorithmFunctionality.h"
#include "GeneticAlgorithmModule.h"

FGeneticAlgorithmThread::FGeneticAlgorithmThread(UGeneticAlgorithmFunctionality* _GAFunctions, int32 _Generations, AGeneticAlgorithmController* _UGAController)
{
	NewThread(_GAFunctions, _Generations, _UGAController);
}

FGeneticAlgorithmThread::FGeneticAlgorithmThread(UGeneticAlgorithmFunctionality* _GAFunctions, TArray<UGenomes*> _CurrentGenerationGenomes, double _GlobalBestFitness, int32 _Generations,
                                                 AGeneticAlgorithmController* _UGAController)
{
	NewThread(_GAFunctions, _Generations, _UGAController);
	GAFunctions->SetCurrentGeneration(_CurrentGenerationGenomes);
	GAFunctions->GlobalBestFitnessScore = _GlobalBestFitness;
}

void FGeneticAlgorithmThread::NewThread(UGeneticAlgorithmFunctionality* _GAFunctions, int32 _Generations, AGeneticAlgorithmController* _UGAController)
{
	
	if(_Generations > 0 && _UGAController)
	{
		Generations = _Generations;
		CurrentGAController = _UGAController;
		bStopThread = false;
		GAFunctions = _GAFunctions;
	}
}

bool FGeneticAlgorithmThread::Init()
{
	bStopThread = false;

	GenerationCount = 0;

	return true;
}

uint32 FGeneticAlgorithmThread::Run()
{
	while(!bStopThread)
	{
		if(GenerationCount < Generations)
		{
			// Invokes a single Epoch - Checks the fitness of the current generation, then creates a new generation.
			GAFunctions->Epoch();

			// Enqueues the fittest Genomes that have been tested. Only those that have beaten the previous highest fitness score are allowed to join.
			CurrentGAController->ThreadNewEnemyQueue.Enqueue(GAFunctions->GetCurrentBestGenomes());
			GenerationCount++;
			// UE_LOG(GeneticAlgorithmModule, Display, TEXT("Generation Number: %d"), GenerationCount);
		}
		else
		{
			// Enqueues the latest generation and best fitness score so far in order for it to be reused later when the thread is reactivated 
			CurrentGAController->ThreadGenomeGenerationQueue.Enqueue(GAFunctions->GetCurrentGeneration());
			CurrentGAController->ThreadGlobalBestFitnessScoreQueue.Enqueue(GAFunctions->GlobalBestFitnessScore);

			bStopThread = true;
		}
	}

	return 0;
}

void FGeneticAlgorithmThread::Stop()
{
	//Any cleanup can be done here
}

void FGeneticAlgorithmThread::AddGenerations(int32 _AdditionalGenerations)
{
	Generations += _AdditionalGenerations;
}
