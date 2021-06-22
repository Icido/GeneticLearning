#include "Genomes.h"

#include "GeneticAlgorithmModule.h"

void UGenomes::InitGenome(int32 _NumBits)
{
	Fitness = 0;
	Bits.Empty();

	if(_NumBits > 56)
		UE_LOG(LogClass, Fatal, TEXT("_NumBits were not initialized properly."));

	if(Bits.Num() > 0)
		UE_LOG(LogClass, Fatal, TEXT("Bits were not emptied."));

	for(int32 i = 0; i < _NumBits; i++)
	{
		/* This seeding initialization makes it consistent - remove at once
		// uint32 Seed1 = 0;
		// uint32 Seed2 = 0;
		// 
		// if (!FApp::bUseFixedSeed)
		// {
		// 	Seed1 = FDateTime::Now().GetHour() * FDateTime::Now().GetDay()  * FDateTime::Now().GetSecond() + FDateTime::Now().GetMillisecond() * (_NumBits + 1);
		// 	Seed2 = FDateTime::Now().GetMonth() * FDateTime::Now().GetMinute() + FDateTime::Now().GetTicks()  * (_NumBits + 1);
		// }
		//
		// FMath::RandInit(Seed1);
		// FMath::SRandInit(Seed2);
		*/

		/* FRandomStream can be used instead of FMath::RandBool() - Check which is better (FMath::RandBool() might be better for boolean rand) 
		// FRandomStream NewRandomStream;
		// NewRandomStream.Initialize(FMath::Rand());
		// bool NewBit = NewRandomStream.RandRange(0, 1) == 0 ? false : true;
		*/
		
		bool NewBit = FMath::RandBool();

		verify(NewBit == true || NewBit == false);
		
		Bits.Push(NewBit);
	}

	if(Bits.Num() > _NumBits)
		UE_LOG(LogClass, Fatal, TEXT("Genomes were not initialized properly."));
}

void UGenomes::AssignBits(TArray<bool> _NewBits)
{
	check(_NewBits.Num() > 0);
	check(_NewBits.Num() == Bits.Num());
	check(Bits.Num() > 0);

	for(int32 bitNum = 0; bitNum < Bits.Num(); bitNum++)
	{
		Bits[bitNum] = _NewBits[bitNum];
	}

	check(Bits.Num() > 0);
	check(_NewBits.Num() == Bits.Num());

	//Bits.Empty();
	//Bits.Append(_NewBits);
}

void UGenomes::AssignSingleBit(int32 _BitIndex, bool _NewBit)
{
	//check(_NewBit == true || _NewBit == false);
	check(Bits.IsValidIndex(_BitIndex));

	Bits[_BitIndex] = _NewBit;

	check(Bits[_BitIndex] == _NewBit);
}

void UGenomes::Verify()
{
	if(Bits.Num() > 56)
		UE_LOG(LogClass, Fatal, TEXT("Number of bits exceeds limit."));
	
	for (int32 i = 0; i < Bits.Num(); i++)
	{
		if(!Bits.IsValidIndex(i))
			UE_LOG(LogClass, Fatal, TEXT("Bit is not accessible."));
		
		if(Bits[i] == false || Bits[i] == true)
			continue;

		UE_LOG(LogClass, Fatal, TEXT("Bit is not a boolean value."));
	}
}

bool UGenomes::ClearGenome()
{
	Bits.Empty();
	Fitness = 0;

	if(Bits.Num() > 0 || Fitness != 0)
		return false;

	return true;
}
