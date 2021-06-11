#include "GeneticLearningCharacter.h"
#include "GeneticLearningProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGeneticLearningCharacter

//TODO: Move On Construction Blueprint functionality out to GLCharacter Blueprint
AGeneticLearningCharacter::AGeneticLearningCharacter()
{
	// ...
}

//TODO: Move BeginPlay Blueprint functionality to GLCharacter Blueprint
void AGeneticLearningCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGeneticLearningCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGeneticLearningCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AGeneticLearningCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGeneticLearningCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGeneticLearningCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGeneticLearningCharacter::LookUpAtRate);
}

//TODO: Move OnFire functionality out to weapon, OnFire calls to held weapon
void AGeneticLearningCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			// const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
			
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			// spawn the projectile at the muzzle
			//World->SpawnActor<AGeneticLearningProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			AGeneticLearningProjectile* NewProjectile = World->SpawnActor<AGeneticLearningProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if(NewProjectile)
			{
				OnFireBP();
				NewProjectile->OnEnemyHit.AddDynamic(this, &AGeneticLearningCharacter::BulletAccuracyInput);
				BulletsFired++;
			}
			else
			{
				UE_LOG(LogFPChar, Error, TEXT("Attempted to fire but no bullet could be created."));
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AGeneticLearningCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGeneticLearningCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGeneticLearningCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGeneticLearningCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGeneticLearningCharacter::BulletAccuracyInput()
{
	BulletsHit++;

	// Fired delegate up to GeneticLearningGameMode if possible - feed PlayerAccuracy through to PlayerData to use in GA SmartEnemyTest
}

float AGeneticLearningCharacter::GetPlayerAccuracy()
{
	return BulletsFired <= 0 ? 0 : 100 * static_cast<float>(BulletsHit) / static_cast<float>(BulletsFired);
} 