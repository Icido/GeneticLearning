#include "GenomeHelper.h"
#include "Genomes.h"

FGenomes* UGenomeHelper::RouletteWheelSelection(TArray<FGenomes*> _CurrentGeneration, double _TotalFitnessScore, int32 _PopulationSize)
{
	const double Slice = FMath::FRand() * _TotalFitnessScore;

	for (int i = 0; i < _PopulationSize; i++)
	{
		if(!_CurrentGeneration.IsValidIndex(i))
		{
			UE_LOG(LogClass, Warning, TEXT("Current Genome selection is not a valid index - Moving to next selection."));
			continue;
		}
		
		//TODO: Find out what causes the first element to become NULL - This warning should be changed back to fatal when found out how to prevent the issue
		//TODO: Issue may stem from Garbage Collection as UGenomes are NOT created as UObjects (due to instability from creating UObjects on non-game thread)
		if(_CurrentGeneration[i] == nullptr || _CurrentGeneration[i]->Fitness == NULL)
		{
			UE_LOG(LogClass, Warning, TEXT("Current Genome selection is NULL selection - Moving to next selection."));
			continue;
		}
		
		_TotalFitnessScore -= _CurrentGeneration[i]->Fitness;

		if (_TotalFitnessScore < Slice)
		{
			return _CurrentGeneration[i];
		}
	}
	
	if(_CurrentGeneration.Last() == nullptr || _CurrentGeneration.Last()->Fitness == NULL)
		return new FGenomes();
	
	return _CurrentGeneration[0];
}

FGenomes* UGenomeHelper::Mutate(FGenomes* _MutatingGenome, double _MutationRate)
{
	FGenomes* MutatedGenome = new FGenomes;
	
	for (int i = 0; i < _MutatingGenome->Bits.Num(); i++)
	{
		if(FMath::FRand() < _MutationRate)
		{
			_MutatingGenome->Bits[i] = _MutatingGenome->Bits[i] == false ? true : false;
		}
	}
	
	MutatedGenome->Bits.Append(_MutatingGenome->Bits);
	
	return MutatedGenome;
}