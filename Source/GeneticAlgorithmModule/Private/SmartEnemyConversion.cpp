#include "SmartEnemyConversion.h"
#include "Genomes.h"

TArray<int32> USmartEnemyConversion::DecodeGenome(FGenomes* _Genomes, int32 _GeneLength)
{
	TArray<int32> MutationValues;

	if(!_Genomes->Bits.IsValidIndex(0))
		UE_LOG(LogClass, Fatal, TEXT("Genomes were not initialized properly."));

	const int32 BitsSize = _Genomes->Bits.Num();

	for(int32 GeneIndex = 0; GeneIndex < BitsSize; GeneIndex += _GeneLength)
	{
		TArray<bool> Gene; // TArray<int32>

		for(int32 BitIndex = 0; BitIndex < _GeneLength; BitIndex++)
		{
			Gene.Push(_Genomes->Bits[GeneIndex + BitIndex]);
		}

		int32 NewMutation = GeneToInt32(Gene);

		MutationValues.Push(NewMutation);
	}

	if(MutationValues.Num() == 0)
		UE_LOG(LogClass, Fatal, TEXT("Mutations were not initialized properly."));
	
	return MutationValues;
}


int32 USmartEnemyConversion::GeneToInt32(TArray<bool> _Gene)
{
	int32 Value = 0;
	int32 Multiplier = 1;

	for(int32 i = _Gene.Num(); i > 0; i--)
	{
		const int32 Bit = _Gene[i - 1] == true ? 1 : 0; // May be safer than a cast 
		Value += Bit * Multiplier;
		Multiplier *= 2;
	}

	return Value;
}


TArray<float> USmartEnemyConversion::DecodedMutations(TArray<int32> _Mutations, int32 _GeneLength)
{
	TArray<float> NewEnemyAttributes;

	const float MaxGeneValue = FMath::Pow(2, _GeneLength); // 255.f, Maximum from 8 bits

	for(int32 i = 0; i < 7; i++)
	{
		if(!_Mutations.IsValidIndex(i))
		{
			UE_LOG(LogClass, Fatal, TEXT("Mutations were not initialized properly."));
		}
	}
	
	const float ArmLength = _Mutations[0];
	const float ClawLength = _Mutations[1];
	const float ThroatLength = _Mutations[2];
	const float AcidStrength = _Mutations[3];
	const float LegLength = _Mutations[4];
	const float AcidSackSize = _Mutations[5];
	const float Constitution = _Mutations[6];

	// Health
	NewEnemyAttributes.Add(50.f  + ((10.f * (MaxGeneValue - AcidSackSize)) / MaxGeneValue) + ((25.0f * Constitution) / MaxGeneValue) + ((15.f * (MaxGeneValue - AcidStrength)) / MaxGeneValue)); //50-100

	// Speed
	NewEnemyAttributes.Add(0.5f + ((1.5f * (MaxGeneValue - LegLength)) / MaxGeneValue) + ((1.f * (MaxGeneValue - Constitution)) / MaxGeneValue)); //0.5-5.5

	
	// Melee Distance
	NewEnemyAttributes.Add(0.5f + ((1.f * ArmLength) / MaxGeneValue) + ((0.5f * LegLength) / MaxGeneValue)); //0.5-3.5

	// Melee Damage
	NewEnemyAttributes.Add(2.0f + ((2.f * ClawLength) / MaxGeneValue)); //2-6

	// Melee Attack Rate
	NewEnemyAttributes.Add(0.5f + ((1.25f * (MaxGeneValue - ArmLength)) / MaxGeneValue) + ((1.f * (MaxGeneValue - ClawLength)) / MaxGeneValue)); //0.5-5 seconds


	// Range Distance
	NewEnemyAttributes.Add(10.f  + ((2.5f * ThroatLength) / MaxGeneValue) + ((1.f * AcidSackSize) / MaxGeneValue)); //10-17

	// Range Damage
	NewEnemyAttributes.Add(1.5f + ((2.f * AcidStrength) / MaxGeneValue)); //1.5-5.5

	// Range Attack Rate
	NewEnemyAttributes.Add(0.5f + ((0.75f * (MaxGeneValue - ThroatLength)) / MaxGeneValue) + ((0.5f * (MaxGeneValue - AcidStrength)) / MaxGeneValue)); //0.5-3 seconds

	return NewEnemyAttributes;
}


TArray<float> USmartEnemyConversion::GenomeToEnemy(FGenomes* _Genomes, int32 _GeneLength)
{
	if(!_Genomes)
		UE_LOG(LogClass, Fatal, TEXT("Genomes could not be read in Conversion."));

	const TArray<int32> DecodedGenomes = DecodeGenome(_Genomes, _GeneLength);

	return DecodedMutations(DecodedGenomes, _GeneLength);
}