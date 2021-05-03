#include "GeneticAlgorithmFunctionality.h"

#include "GeneticAlgorithmModule.h"
#include "GenomeHelper.h"
#include "Genomes.h"
#include "SmartEnemyConversion.h"
#include "SmartEnemyTest.h"

UGeneticAlgorithmFunctionality::UGeneticAlgorithmFunctionality(): Super()
{
	CurrentGenerationGenomes.Empty();
	Children.Empty();
	
	for (int32 i = 0; i < PopulationSize; i++)
	{
		//FGenomes* Child = new FGenomes(ChromosomeLength);
		UGenomes* Child = NewObject<UGenomes>();
		Child->InitGenome(ChromosomeLength);

		Child->Verify();

		CurrentGenerationGenomes.Push(Child);

		UGenomes* SecondChild = NewObject<UGenomes>();
		Children.Push(SecondChild);
	}

	GHelper = NewObject<UGenomeHelper>();
	SEConvert = NewObject<USmartEnemyConversion>();
	SETest = NewObject<USmartEnemyTest>();
	Mother = NewObject<UGenomes>();
   	Father = NewObject<UGenomes>();
   	Child1 = NewObject<UGenomes>();
   	Child2 = NewObject<UGenomes>();
}

void UGeneticAlgorithmFunctionality::SetCurrentGeneration(TArray<UGenomes*> _LastGenerationGenomes)
{
	CurrentGenerationGenomes.Empty();
	CurrentGenerationGenomes.Append(_LastGenerationGenomes);
	Children.Empty();
	Children.Append(_LastGenerationGenomes);
}

TArray<UGenomes*> UGeneticAlgorithmFunctionality::GetCurrentGeneration() const
{
	return CurrentGenerationGenomes;
}

TArray<TArray<float>> UGeneticAlgorithmFunctionality::GetCurrentBestGenomes() const
{
	return BestAvailableGenomesDecoded;
}

void UGeneticAlgorithmFunctionality::CopyChildren()
{
	for(int32 i = 0; i < PopulationSize; i++)
	{
		check(CurrentGenerationGenomes[i]->Bits.Num() == ChromosomeLength);
		check(Children[i]->Bits.Num() == ChromosomeLength);
		
		CurrentGenerationGenomes[i]->AssignBits(Children[i]->Bits);
	}
}

void UGeneticAlgorithmFunctionality::CheckGeneration()
{
	check(CurrentGenerationGenomes.Num() == PopulationSize);
	
	for(int32 i = 0; i < PopulationSize; i++)
	{
		check(CurrentGenerationGenomes.IsValidIndex(i));
		check(CurrentGenerationGenomes[i] != NULL);
		check(CurrentGenerationGenomes[i] != nullptr);
		//check(CurrentGenerationGenomes[i]->Fitness != NAN);
		//check(CurrentGenerationGenomes[i]->Fitness != NULL);
		check(CurrentGenerationGenomes[i]->Fitness >= 0);
		check(CurrentGenerationGenomes[i]->Fitness <= 1);
		check(CurrentGenerationGenomes[i]->Bits.Num() == ChromosomeLength);
	}
}


void UGeneticAlgorithmFunctionality::Epoch()
{
	CheckGeneration();
	
	// Updates the Fitness Scores of the Current Generation
	UpdateFitnessScores();

	CheckGeneration();
	// If there's been no perfection yet (according to the UpdateFitnessScores() - Create a new generation
	if(!bHasFoundPerfection)
	{
		int32 NumberOfNewChildren = 0;

		CheckGeneration();
		
		while(NumberOfNewChildren < PopulationSize)
		{
			CheckGeneration();
			
			//FGenomes* Mother = GHelper->RouletteWheelSelection(CurrentGenerationGenomes, TotalFitnessScore, PopulationSize);
			//FGenomes* Father = GHelper->RouletteWheelSelection(CurrentGenerationGenomes, TotalFitnessScore, PopulationSize);
			Mother->AssignBits(GHelper->RouletteWheelSelection(CurrentGenerationGenomes, TotalFitnessScore, PopulationSize));
			Father->AssignBits(GHelper->RouletteWheelSelection(CurrentGenerationGenomes, TotalFitnessScore, PopulationSize));

			CheckGeneration();
			
			check(Mother->Bits.Num() == ChromosomeLength);
			check(Father->Bits.Num() == ChromosomeLength);
			
			Crossover(Mother, Father, Child1, Child2);

			check(Child1->Bits.Num() == ChromosomeLength);
			check(Child2->Bits.Num() == ChromosomeLength);
			
			CheckGeneration();
			
			GHelper->Mutate(Child1, MutationRate);
			GHelper->Mutate(Child2, MutationRate);

			check(Child1->Bits.Num() == ChromosomeLength);
			check(Child2->Bits.Num() == ChromosomeLength);

			CheckGeneration();
			
			Children[NumberOfNewChildren]->AssignBits(Child1->Bits);
			Children[NumberOfNewChildren + 1]->AssignBits(Child2->Bits);

			CheckGeneration();
			
			NumberOfNewChildren += 2;
		}

		CheckGeneration();
		
		CopyChildren();

		CheckGeneration();
		
		GenerationNumber++;
	}
}

void UGeneticAlgorithmFunctionality::Crossover(UGenomes* _Mother, UGenomes* _Father, UGenomes* _Child1, UGenomes* _Child2) const
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
		check(CurrentGenerationGenomes[i] != nullptr);
		check(CurrentGenerationGenomes.IsValidIndex(i));
		
		//CurrentGenerationGenomes[i]->Verify();

		CurrentGenerationGenomes[i]->Fitness = 0;
		
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
