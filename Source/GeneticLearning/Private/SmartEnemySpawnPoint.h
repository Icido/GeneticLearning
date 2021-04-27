#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmartEnemySpawnPoint.generated.h"

UCLASS()
class GENETICLEARNING_API ASmartEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartEnemySpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetSpawnLocation() const {return SpawnLocation;}

	FRotator GetSpawnRotation() const {return SpawnRotation;}
	
private:
	FVector SpawnLocation;
	FRotator SpawnRotation;
};
