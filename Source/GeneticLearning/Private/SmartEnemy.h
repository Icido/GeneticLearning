#pragma once

#include "CoreMinimal.h"


#include "InteractInterface.h"
#include "SmartEnemyStats.h"
#include "GameFramework/Actor.h"
#include "SmartEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSmartEnemyDeath, class ASmartEnemy*, DeadSmartEnemy);

UCLASS()
class GENETICLEARNING_API ASmartEnemy : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USmartEnemyStats* Stats;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnBulletHit_Implementation(float Damage) override;	
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyHurt();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyDeath();
	
	UFUNCTION(BlueprintCallable)
	void FireDeathDelegate();
	
	UPROPERTY(BlueprintAssignable)
	FSmartEnemyDeath OnDeath;

	UFUNCTION(BlueprintCallable)
	void AssignStats(TArray<float> _NewStats) const;
};

