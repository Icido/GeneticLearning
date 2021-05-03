#include "GenomeHelper.h"
#include "Genomes.h"
#include "GeneticAlgorithmModule.h"

UGenomeHelper::UGenomeHelper() : Super()
{
	//TODO: Stop using this DefaultGenome if not used - currently uses hard-coded chromosome number
	DefaultGenome = NewObject<UGenomes>();
	DefaultGenome->InitGenome(56);
}

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
		
		//TODO: Find out what causes the first element to become NULL - This warning should be changed back to fatal when found out how to prevent the issue
		//TODO: Issue may stem from Garbage Collection as UGenomes are NOT created as UObjects (due to instability from creating UObjects on non-game thread)
		// check(_CurrentGeneration[i] != nullptr);
		// check(_CurrentGeneration[i]->Fitness != NULL);
		// check(_CurrentGeneration[i]->Bits.Num() != 0);
		// check(_CurrentGeneration[i]->Bits.Num() == 56);
		// check(_CurrentGeneration[i]->Fitness > 1);
		// check(_CurrentGeneration[i]->Fitness < 0);
		
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
		return DefaultGenome->Bits;
	
	return _CurrentGeneration[0]->Bits;
}

void UGenomeHelper::Mutate(UGenomes* _MutatingGenome, double _MutationRate)
{
	//FGenomes* MutatedGenome = new FGenomes;
	//MutatedGenome->ClearGenome();
	
	for (int i = 0; i < _MutatingGenome->Bits.Num(); i++)
	{
		if(FMath::FRand() < _MutationRate)
		{
			_MutatingGenome->Bits[i] = !_MutatingGenome->Bits[i];
		}
	}
	
	//MutatedGenome->Bits.Append(_MutatingGenome->Bits);
	
	//return MutatedGenome;
}