#include "SmartEnemyTest.h"

// TODO: Reimplement in a method that the user can determine the Test
double USmartEnemyTest::FitnessTest(TArray<float> _SmartEnemyAttributes) const
{
	int32 Timeout = 50;
	
	//TODO: Move the player statistics elsewhere, where they can be assigned to and retrieved
	float PlayerAccuracy = 80.f;
	float PlayerDamage = 10.f;
	float PlayerRange = 25.f;
	float PlayerAttackRate = 2.f;
	float PlayerCurrentHealth = 100.f;
	float PlayerMaxHealth = PlayerCurrentHealth;

	float SEMeleeDamagePerSecond = _SmartEnemyAttributes[3] * _SmartEnemyAttributes[4];
	float SERangeDamagePerSecond = _SmartEnemyAttributes[6] * _SmartEnemyAttributes[7];

	float PlayerDamagePerSecond = PlayerDamage * PlayerAttackRate;
	float CurrentSmartEnemyHealth = _SmartEnemyAttributes[0];

	float StartDistance = FMath::RandRange(10, 30);

	bool bIsPlayerDead = false;
	bool bPrefersMelee = (SEMeleeDamagePerSecond > SERangeDamagePerSecond) ? true : false;

	while(CurrentSmartEnemyHealth > 0)
	{
		//Checks if the player is in range of shooting the zombie
		if (StartDistance > PlayerRange)
		{
			//Enemy moves towards player
			StartDistance -= _SmartEnemyAttributes[1];

			//Player moves away from enemy at a speed of 1 unit per round
			StartDistance += 1;
			
			Timeout--;

			// Stops the test if nothing happens to either the 
			if(Timeout <= 0)
			{
				break;
			}
			
			continue;
		}

		//Takes damage from player (if the player hits)
		if (FMath::RandRange(0, 100) <= PlayerAccuracy)
		{
			CurrentSmartEnemyHealth -= PlayerDamagePerSecond;
		}

		//If the zombie is not dead, deal damage to player (update player damage taken), else BREAK
		if(CurrentSmartEnemyHealth <= 0)
		{
			break;
		}
		
		if(bPrefersMelee)
		{
			if(StartDistance <= _SmartEnemyAttributes[2])
			{
				PlayerCurrentHealth -= SEMeleeDamagePerSecond;
			}
			else
			{
				//Move forward
				StartDistance -= _SmartEnemyAttributes[1];
			}
		}
		else
		{
			if (StartDistance <= _SmartEnemyAttributes[5])
			{
				PlayerCurrentHealth -= SERangeDamagePerSecond;
			}
			else
			{
				//Move forward
				StartDistance -= _SmartEnemyAttributes[1];
			}
		}
		
            
		//Check if player is dead, BREAK
		if (PlayerCurrentHealth <= 0)
		{
			bIsPlayerDead = true;
			break;
		}

	}

	if(bIsPlayerDead)
	{
		return 1;
	}

	//TODO: Rework Fitness score to take into account damage done, instead of percentage of health done to player
	const double NewFitness = (PlayerMaxHealth - PlayerCurrentHealth) / PlayerMaxHealth;
	
	return NewFitness <= 0 ? 0.001f : NewFitness;
}
