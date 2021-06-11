// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultFPWeapon.h"
#include "GeneticLearningProjectile.h"

void ADefaultFPWeapon::Fire(const FRotator SpawnRotation)
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((MuzzleComponent != nullptr) ? MuzzleComponent->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
			
			//const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			// spawn the projectile at the muzzle
			//World->SpawnActor<AGeneticLearningProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			AGeneticLearningProjectile* NewProjectile = World->SpawnActor<AGeneticLearningProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if(NewProjectile)
			{
				PlayFireEffects();
				//NewProjectile->OnEnemyHit.AddDynamic(this, &AGeneticLearningCharacter::BulletAccuracyInput);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Attempted to fire but no bullet could be created."));
			}
		}
	}
}
