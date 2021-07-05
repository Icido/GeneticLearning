#include "GeneticAlgorithmThread.h"
#include "GeneticAlgorithmResults.h"
#include "GeneticAlgorithmFunctionality.h"
#include "GeneticAlgorithmModule.h"

FGeneticAlgorithmThread::FGeneticAlgorithmThread(UGeneticAlgorithmFunctionality* _GAFunctions, int32 _Generations, AGeneticAlgorithmResults* _AGAResults)
{
	NewThread(_GAFunctions, _Generations, _AGAResults);

	if(!_AGAResults->IsCurrentGenerationEmpty())
	{
		GAFunctions->SetCurrentGeneration(_AGAResults->RetrieveCurrentGenerationFromResults());
		GAFunctions->GlobalBestFitnessScore = _AGAResults->RetrieveGlobalBestFitnessFromResults();
	}
}

void FGeneticAlgorithmThread::NewThread(UGeneticAlgorithmFunctionality* _GAFunctions, int32 _Generations, AGeneticAlgorithmResults* _AGAResults)
{
	
	if(_Generations > 0 && _AGAResults)
	{
		Generations = _Generations;
		CurrentGAResults = _AGAResults;
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
			CurrentGAResults->ThreadNewEnemyQueue.Enqueue(GAFunctions->GetCurrentBestGenomes());
			GenerationCount++;
			// UE_LOG(GeneticAlgorithmModule, Display, TEXT("Generation Number: %d"), GenerationCount);
		}
		else
		{
			// Enqueues the latest generation and best fitness score so far in order for it to be reused later when the thread is reactivated 
			CurrentGAResults->ThreadGenomeGenerationQueue.Enqueue(GAFunctions->GetCurrentGeneration());
			CurrentGAResults->ThreadGlobalBestFitnessScoreQueue.Enqueue(GAFunctions->GlobalBestFitnessScore);

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
