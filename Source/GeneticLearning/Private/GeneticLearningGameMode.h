#pragma once

#include "CoreMinimal.h"

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

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	
	UPROPERTY(VisibleAnywhere)
	class AGeneticAlgorithmController* GAController;

private:
	UPROPERTY(EditAnywhere)
	TArray<ASmartEnemy*> SpawnedSmartEnemies;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASmartEnemy> SmartEnemyBP;

};



