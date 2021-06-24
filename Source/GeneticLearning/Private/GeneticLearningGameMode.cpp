#include "GeneticLearningGameMode.h"

#include "GeneticLearningHUD.h"
#include "GeneticLearningCharacter.h"
#include "GeneticAlgorithmController.h"
#include "SmartEnemySpawnPoint.h"
#include "ToolBuilderUtil.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


AGeneticLearningGameMode::AGeneticLearningGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character - TODO: Modify to not use hard-coded destination and use in BP instead
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGeneticLearningHUD::StaticClass();

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGeneticLearningGameMode::BeginPlay()
{
	Super::BeginPlay();

	//To gather all Spawn Points for reference
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmartEnemySpawnPoint::StaticClass(), SpawnPoints);
	
	GAController = NewObject<AGeneticAlgorithmController>();
	GAController->Setup();

	GAController->InitGenerations(5000);
	
	// Assert to ensure there's no leftover SmartEnemies that may disrupt initialization
	check(AllSmartEnemiesDead());
	
	WaveInitialization();
}

void AGeneticLearningGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGeneticLearningGameMode::WaveCheck(ASmartEnemy* LastSmartEnemyKilled)
{
	GAController->InitGenerations(1000);

	if(LastSmartEnemyKilled != nullptr)
	{
		const int32 EnemyIndex = SpawnedSmartEnemies.IndexOfByPredicate([&LastSmartEnemyKilled](const ASmartEnemy* InSmartEnemy)
		{
			return InSmartEnemy == LastSmartEnemyKilled; 
	    });

		if(EnemyIndex != INDEX_NONE)
		{
			//Swap with last element, remove, don't change allocation.
			SpawnedSmartEnemies[EnemyIndex]->Destroy();
			SpawnedSmartEnemies.RemoveAtSwap(EnemyIndex, 1, false);
		}
	}
	
	if(AllSmartEnemiesDead())
	{
		WaveInitialization();
	}
}

void AGeneticLearningGameMode::WaveInitialization()
{
	if(SmartEnemyBP != nullptr)
	{
		UWorld* const World = GetWorld();
		if(World != nullptr)
		{
			for(int i = 0; i < SpawnPoints.Num(); i++)
			{
				ASmartEnemy* NewEnemy = GetWorld()->SpawnActor<ASmartEnemy>(SmartEnemyBP, SpawnPoints[i]->GetTransform());
				if(NewEnemy)
				{
					// GameMode does not need to know what enemy it's retrieving, just that it's getting an enemy
					NewEnemy->AssignStats(GAController->GetNewEnemy());
					
					NewEnemy->OnDeath.AddDynamic(this, &AGeneticLearningGameMode::WaveCheck);
					SpawnedSmartEnemies.Add(NewEnemy);
					
				}
				else
				{
					GLog->Log("Failed to spawn new Enemy");
				}
			}
		}
	}
}