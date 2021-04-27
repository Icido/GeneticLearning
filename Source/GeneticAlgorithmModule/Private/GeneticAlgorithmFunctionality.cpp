#include "GeneticAlgorithmFunctionality.h"

#include "GeneticAlgorithmModule.h"
#include "GenomeHelper.h"
#include "Genomes.h"
#include "SmartEnemyConversion.h"
#include "SmartEnemyTest.h"

UGeneticAlgorithmFunctionality::UGeneticAlgorithmFunctionality(): Super()
{
	CurrentGenerationGenomes.Empty();

	for (int32 i = 0; i < PopulationSize; i++)
	{
		FGenomes* Child = new FGenomes(ChromosomeLength);

		Child->Verify();

		CurrentGenerationGenomes.Push(Child);
	}

	GHelper = NewObject<UGenomeHelper>();
	SEConvert = NewObject<USmartEnemyConversion>();
	SETest = NewObject<USmartEnemyTest>();
}

void UGeneticAlgorithmFunctionality::SetCurrentGeneration(TArray<FGenomes*> _LastGenerationGenomes)
{
	CurrentGenerationGenomes.Empty();
	CurrentGenerationGenomes.Append(_LastGenerationGenomes);
}

TArray<FGenomes*> UGeneticAlgorithmFunctionality::GetCurrentGeneration() const
{
	return CurrentGenerationGenomes;
}

TArray<TArray<float>> UGeneticAlgorithmFunctionality::GetCurrentBestGenomes() const
{
	return BestAvailableGenomesDecoded;
}


void UGeneticAlgorithmFunctionality::Epoch()
{
	// Updates the Fitness Scores of the Current Generation
	UpdateFitnessScores();

	// If there's been no perfection yet (according to the UpdateFitnessScores() - Create a new generation
	if(!bHasFoundPerfection)
	{
		int32 NumberOfNewChildren = 0;

		TArray<FGenomes*> Children;

		while(NumberOfNewChildren < PopulationSize)
		{	
			FGenomes* Mother = GHelper->RouletteWheelSelection(CurrentGenerationGenomes, TotalFitnessScore, PopulationSize);
			FGenomes* Father = GHelper->RouletteWheelSelection(CurrentGenerationGenomes, TotalFitnessScore, PopulationSize);
			
			FGenomes* Child1 = new FGenomes;
			FGenomes* Child2 = new FGenomes;
			
			Crossover(Mother, Father, Child1, Child2);

			GHelper->Mutate(Child1, MutationRate);
			GHelper->Mutate(Child2, MutationRate);

			Children.Push(Child1);
			Children.Push(Child2);
			
			NumberOfNewChildren += 2;
		}

		CurrentGenerationGenomes.Empty();
		CurrentGenerationGenomes.Append(Children);

		GenerationNumber++;
	}
}

void UGeneticAlgorithmFunctionality::Crossover(FGenomes* _Mother, FGenomes* _Father, FGenomes* _Child1, FGenomes* _Child2) const
{
	_Child1->Bits.Empty();
	_Child2->Bits.Empty();
	
	//If it's above crossover rate or parents are the same, copy through
	if(FMath::FRand() > CrossoverRate || _Mother->Bits == _Father->Bits)
	{
		_Child1->Bits.Append(_Mother->Bits);
		_Child2->Bits.Append(_Father->Bits);
		
		return;
	}

	const int32 CrossoverPoint = FMath::RandRange(0, ChromosomeLength - 1);

	for (int i = 0; i < CrossoverPoint; i++)
	{
		_Child1->Bits.Push(_Father->Bits[i]);
		_Child2->Bits.Push(_Mother->Bits[i]);
	}

	for (int i = CrossoverPoint; i < _Father->Bits.Num() && i < _Mother->Bits.Num(); i++)
	{
		_Child1->Bits.Push(_Mother->Bits[i]);
		_Child2->Bits.Push(_Father->Bits[i]);
	}
}

void UGeneticAlgorithmFunctionality::UpdateFitnessScores()
{
	FittestGenomeIndex = 0;
	BestFitnessScore = 0;
	TotalFitnessScore = 0;

	for (int i = 0; i < PopulationSize; i++)
	{
		//Converts the genes of this individual into a list of attribute values, used in the fitness test
		if(CurrentGenerationGenomes[i] == nullptr || !CurrentGenerationGenomes.IsValidIndex(i))
			UE_LOG(GeneticAlgorithmModule, Fatal, TEXT("Genomes were not initialized properly."));
		
		CurrentGenerationGenomes[i]->Verify();
		
		CurrentGenerationGenomes[i]->Fitness = SETest->FitnessTest(SEConvert->GenomeToEnemy(CurrentGenerationGenomes[i], GeneLength));
		
		TotalFitnessScore += CurrentGenerationGenomes[i]->Fitness;

		if(CurrentGenerationGenomes[i]->Fitness > GlobalBestFitnessScore)
		{
			GlobalBestFitnessScore = CurrentGenerationGenomes[i]->Fitness;
			BestGenomes = CurrentGenerationGenomes[i];
			GenerationOfGlobalBestFitnessScore = GenerationNumber;
			
			BestAvailableGenomesDecoded.Push(SEConvert->GenomeToEnemy(CurrentGenerationGenomes[i], GeneLength));

			if (GlobalBestFitnessScore < 1 && GlobalBestFitnessScore > 0.95)
			{
				GenerationCountdown = GenerationNumber + 1000;
			}
		}

		if(GenerationCountdown == GenerationNumber)
		{
			bHasFoundPerfection = true;
			return;
		}

		if (CurrentGenerationGenomes[i]->Fitness > BestFitnessScore)
		{
			BestFitnessScore = CurrentGenerationGenomes[i]->Fitness;
			FittestGenomeIndex = i;

			//Check if (Fitness = 1) || (Fitness ~ 0.95 for many generations)
			if (CurrentGenerationGenomes[i]->Fitness == 1)
			{
				bHasFoundPerfection = true;
				BestGenomes = CurrentGenerationGenomes[i];
				return;
			}
		}
	}
}
