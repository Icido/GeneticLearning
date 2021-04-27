#include "GeneticLearningGameMode.h"

#include "GeneticLearningHUD.h"
#include "GeneticLearningCharacter.h"
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

	GAController = CreateDefaultSubobject<UGeneticAlgorithmController>(TEXT("GA Controller"));
}

void AGeneticLearningGameMode::BeginPlay()
{
	Super::BeginPlay();

	//To gather all Spawn Points for reference
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmartEnemySpawnPoint::StaticClass(), SpawnPoints);

	GAController->InitGenerations(1000);
	
	// Assert to ensure there's no leftover SmartEnemies that may disrupt initialization
	check(AllSmartEnemiesDead());
	
	WaveInitialization();
}

void AGeneticLearningGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*
	Wave Controller:
	void Spawn New Enemies - called each time a smart enemy dies (cleaner to do so) - DELEGATE - DONE
		If all Smart Enemies are dead:
			Gather GA data - DONE
			Spawn new enemies with renewed data - DONE
	
	Smart Enemy created:
		Takes in new data from Wave Controller - DONE
		Assigns stats and subsequent attack preference - DONE
		Begin AI attacking - Do after GA is working, SmartEnemyTest should reflect AI
	
	Smart Enemy death:
		Send current player data to be fed into the GA - TODO: Figure out what information would be relevant
		Shout delegate to Wave Controller - DELEGATE - DONE
	
	GA:
		GAController - Invokes multi-generational epochs to obtain best enemy mutations - Useless, only invoked, did not have any value. - DONE
		GA - Contains fitness scores of enemies along with mutation encoded as bits, mutates a generation per epoch - DONE
		Genomes - Initialised as brand new randomized values of bits to act as the first generation - DONE
		EnemyTest - A loop that the selected enemy is put through to test its fitness (by simulating fighting the player) - DONE
		SmartEnemySetup - Helper functions to decode genes and alter them to usable attributes for the SmartEnemy - DONE
		SmartEnemy - An enemy with allocated attributes and stats, no knowledge of any other data - DONE
	*/
}

void AGeneticLearningGameMode::WaveCheck(ASmartEnemy* LastSmartEnemyKilled)
{
	GAController->InitGenerations(100);

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