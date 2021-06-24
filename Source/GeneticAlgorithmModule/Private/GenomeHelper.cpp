#include "GenomeHelper.h"
#include "Genomes.h"
#include "GeneticAlgorithmModule.h"

TArray<bool> UGenomeHelper::RouletteWheelSelection(TArray<UGenomes*> _CurrentGeneration, double _TotalFitnessScore, int32 _PopulationSize)
{
	const double Slice = FMath::FRand() * _TotalFitnessScore;

	for (int i = 0; i < _PopulationSize; i++)
	{
		if(!_CurrentGeneration.IsValidIndex(i))
		{
			UE_LOG(GeneticAlgorithmModule, Error, TEXT("Current Genome selection is not a valid index - Moving to next selection."));
			continue;
		}
		
		if(_CurrentGeneration[i] == nullptr || _CurrentGeneration[i]->Fitness == NULL)
		{
			UE_LOG(GeneticAlgorithmModule, Error, TEXT("Current Genome selection is NULL selection - Moving to next selection."));
			continue;
		}
		
		_TotalFitnessScore -= _CurrentGeneration[i]->Fitness;

		if (_TotalFitnessScore < Slice)
		{
			return _CurrentGeneration[i]->Bits;
		}
	}
	
	if(_CurrentGeneration.Last() == nullptr || _CurrentGeneration.Last()->Fitness == NULL)
		UE_LOG(GeneticAlgorithmModule, Error, TEXT("Last Genome selection is NULL selection - selecting First Genome."));

	if(_CurrentGeneration[0] == nullptr || _CurrentGeneration[0]->Fitness == NULL)
		UE_LOG(GeneticAlgorithmModule, Fatal, TEXT("First Genome selection is NULL selection - cannot select any Genome."));

	
	return _CurrentGeneration[0]->Bits;
}

void UGenomeHelper::Mutate(UGenomes* _MutatingGenome, double _MutationRate)
{	
	for (int i = 0; i < _MutatingGenome->Bits.Num(); i++)
	{
		if(FMath::FRand() < _MutationRate)
		{
			_MutatingGenome->Bits[i] = !_MutatingGenome->Bits[i];
		}
	}
}