#include "SmartEnemyStats.h"

// Sets default values for this component's properties
USmartEnemyStats::USmartEnemyStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	//Turned off as unneeded at this time
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USmartEnemyStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USmartEnemyStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USmartEnemyStats::TakeHealthDamage(float Damage)
{
	Stats.Health -= Damage;
}

bool USmartEnemyStats::IsDead() const
{
	return Stats.Health <= 0;
}

void USmartEnemyStats::AssignStats(TArray<float> _Stats)
{
	if(_Stats.Num() != 8)
	{
		UE_LOG(LogTemp, Error, TEXT("Number of Stats does not match required stats: %d"), _Stats.Num());
		return;
	}
	
	Stats.Health = _Stats[0];
	Stats.Speed = _Stats[1];
	Stats.MeleeDistance = _Stats[2];
	Stats.MeleeDamage = _Stats[3];
	Stats.MeleeAttackRate = _Stats[4];
	Stats.RangeDistance = _Stats[5];
	Stats.RangeDamage = _Stats[6];
	Stats.RangeAttackRate = _Stats[7];
}

 