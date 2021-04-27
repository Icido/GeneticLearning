#include "SmartEnemySpawnPoint.h"

// Sets default values
ASmartEnemySpawnPoint::ASmartEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmartEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnLocation = this->GetActorLocation();
	SpawnRotation = this->GetActorRotation();
}

// Called every frame
void ASmartEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

