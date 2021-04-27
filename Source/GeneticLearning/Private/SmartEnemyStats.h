#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmartEnemyStats.generated.h"

USTRUCT()
struct GENETICLEARNING_API FEnemyStats
{
	GENERATED_BODY()
	
	float Health;
	float Speed;
	
	float RangeDistance;
	float RangeDamage;
	float RangeAttackRate;
	
	float MeleeDistance;
	float MeleeDamage;
	float MeleeAttackRate;
};

/*
USTRUCT()
struct GENETICLEARNING_API FEnemyMutations
{
	GENERATED_BODY()
	
	int ArmLength;
	int ClawLength;
	int ThroatLength;
	int AcidStrength;
	int LegLength;
	int AcidSackSize;
	int Constitution;
};
*/

//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS( Blueprintable )
class GENETICLEARNING_API USmartEnemyStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USmartEnemyStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeHealthDamage(float Damage);

	bool IsDead() const;

	void AssignStats(TArray<float> _Stats);

	UPROPERTY(EditAnywhere)
    FEnemyStats Stats {10, 1, 1, 1, 1, 1, 1, 1};
};
