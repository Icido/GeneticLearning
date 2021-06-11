// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "DefaultFPWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ADefaultFPWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void Fire(const FRotator SpawnRotation) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void PlayFireEffects() override;
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AGeneticLearningProjectile> ProjectileClass;
};
