#pragma once

#include "CoreMinimal.h"

#include "GeneticAlgorithmController.h"
#include "SmartEnemy.h"
#include "GameFramework/GameModeBase.h"
#include "GeneticLearningGameMode.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FSmartEnemyDestroyed, ASmartEnemy*, DestroyedSmartEnemy);

UCLASS(minimalapi)
class AGeneticLearningGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGeneticLearningGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/**
	 * @brief Checks if there are any SmartEnemies within the SpawnedSmartEnemies Array
	 * @return True if SpawnedSmartEnemies is empty
	 */
	UFUNCTION(BlueprintCallable)
	bool AllSmartEnemiesDead() const {return SpawnedSmartEnemies.Num() <= 0;}

	UFUNCTION(BlueprintCallable)
	void WaveCheck(ASmartEnemy* LastSmartEnemyKilled);

	UFUNCTION(BlueprintCallable)
	void WaveInitialization();
	

private:
	UPROPERTY(EditAnywhere)
	TArray<ASmartEnemy*> SpawnedSmartEnemies;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASmartEnemy> SmartEnemyBP;

	UPROPERTY()
	UGeneticAlgorithmController* GAController;
};



