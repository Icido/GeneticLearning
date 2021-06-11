#include "SmartEnemy.h"

// Sets default values
ASmartEnemy::ASmartEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Stats = CreateDefaultSubobject<USmartEnemyStats>(TEXT("Enemy Stats"));
	if(!Stats)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Stats not created!"));
	}
}

// Called when the game starts or when spawned
void ASmartEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASmartEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASmartEnemy::OnBulletHit_Implementation(float Damage)
{
	if(Stats == nullptr)
	{
		//GLog->Log("No Stats found.");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("No Stats found."));
		return;
	}

	if(Stats->Stats.Health <= 0)
	{
		//GLog->Log("Enemy should have been destroyed.");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enemy should have been destroyed."));

		OnEnemyDeath();
		return;
	}
	
	Stats->TakeHealthDamage(Damage);

	//GLog->Log(TEXT("Enemy has taken some damage."));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("Enemy has taken %f damage, %f health remains."), Damage, Stats->Stats.Health));
	UE_LOG(LogTemp, Display, TEXT("Enemy has taken %f damage, %f health remains."), Damage, Stats->Stats.Health);
	
		
	if(Stats->IsDead())
	{
		OnEnemyDeath();
	}
	else
	{
		OnEnemyHurt();
	}
}

void ASmartEnemy::FireDeathDelegate()
{
	// Call Delegate to WaveManager notifying death
	OnDeath.Broadcast(this);
}

void ASmartEnemy::AssignStats(TArray<float> _NewStats) const
{
	Stats->AssignStats(_NewStats);
}


