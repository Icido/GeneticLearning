#pragma once

#include "CoreMinimal.h"

#include "GenomeHelper.h"
#include "SmartEnemyConversion.h"
#include "SmartEnemyTest.h"
#include "UObject/NoExportTypes.h"
#include "GeneticAlgorithmFunctionality.generated.h"


// Forward declarations
class FGenomes;

/**
 * @brief The functionality behind the Genetic Algorithm - this class utilises the Genetic Algorithm and returns any Genomes fit enough for export
 */
UCLASS()
class GENETICALGORITHMMODULE_API UGeneticAlgorithmFunctionality : public UObject
{
public:
	GENERATED_BODY()
	UGeneticAlgorithmFunctionality();

	/**
	 * @brief Set function that sets the current Generation of Genomes to the Generation that has been given
	 * @param _LastGenerationGenomes The TArray of Genomes to be used as the current Generation
	 */
	void SetCurrentGeneration(TArray<FGenomes*> _LastGenerationGenomes);

	/**
	* @returns Latest Generation of Genomes that were created
	*/
	TArray<FGenomes*> GetCurrentGeneration() const;

	/**
	* @returns Any Genomes that have been the "fittest" at one point 
	*/
	TArray<TArray<float>> GetCurrentBestGenomes() const;

	/**
	* @brief The encompassing function that tests the current generation and creates a new generation if it has not reached perfection
	*/
	void Epoch();
	
	/**
	* @brief The fitness scores are updated within this function, each Genome is tested and assigned a fitness score between 0 and 1
	*/
	void UpdateFitnessScores();

	/**
	 * @brief The Mother and Father Genomes exchange a chunk of their bits to pass down to Child1 and Child2, depending on the crossover rate
	 * @param _Mother The first Parent Genome selected to pass their Genes down
	 * @param _Father The second Parent Genome selected to pass their Genes down
	 * @param _Child1 The first Child Genome that receives the Genes
	 * @param _Child2 The second Child Genome that receives the Genes
	 */
	void Crossover(FGenomes* _Mother, FGenomes* _Father, FGenomes* _Child1, FGenomes* _Child2) const;
	
	// Different helper classes used for gene editing, gene conversion and testing respectively
	UPROPERTY()
	UGenomeHelper* GHelper;
	UPROPERTY()
	USmartEnemyConversion* SEConvert;
	UPROPERTY()
	USmartEnemyTest* SETest;


	/** @brief Whether or not the Genetic Algorithm has found a perfect Genome for the test, one that has achieved over 0.95 for long enough or exactly 1 */
	bool bHasFoundPerfection = false;
	

	
	/** @brief Size of each Generation's population */
	const int32 PopulationSize = 1000; 

	/** @brief Size of each attribute - 8 bits = [0 - 255] */
	const int32 GeneLength = 8;

	/** @brief Number of mutations each Genome contains (7 mutations, 8 Smart Enemy attributes - look @SmartEnemyConversion to check) */
	const int32 NumGenesPerLength = 7;

	/** @brief Size of each Genome's bits (GeneLength * NumGenesPerLength) */
	const int32 ChromosomeLength = GeneLength * NumGenesPerLength;

	/** @brief The rate at which Crossover occurs [0-1] (if FRand is above CrossoverRate, parents are copied directly to children) */
	const double CrossoverRate = 0.7f;

	/** @brief The rate at which Mutation occurs [0-1] (if FRand is below MutationRate, mutates the bit) */
	const double MutationRate = 0.001f;
	

	
	/** @brief Number of Generations that has occured - used for keeping track in case there's not been any progress above 0.95 fitness */
	UPROPERTY()
	int32 GenerationNumber = 0;

	/** @brief Generation of the current Fittest Genome across all generations - used for keeping track in case there's not been any progress above 0.95 fitness */
	UPROPERTY()
	int32 GenerationOfGlobalBestFitnessScore = 0;

	/** @brief A goal generation for if there's been no progress above 0.95 - when the number of generations hits this goal, the Genetic Algorithm stops */
	UPROPERTY()
	int32 GenerationCountdown = -1;

	
	
	/** @brief Index of the Fittest Genome of that Generation */
	UPROPERTY()
	int32 FittestGenomeIndex = 0;

	/** @brief Best Fitness Score is only the best of that Generation */
	UPROPERTY()
	double BestFitnessScore = 0;

	/** @brief Total Fitness Score is the sum of all Fitness Scores of a Generation */
	UPROPERTY()
	double TotalFitnessScore = 0;

	/** @brief Global Best Fitness Score is persistent across all Generations, even if the thread is finished */
	UPROPERTY()
	double GlobalBestFitnessScore = 0;

	

	/** @brief The fittest Genomes that have been found - if a new Genome is fitter, this is replaced by the fitter Genome */
	FGenomes* BestGenomes = nullptr;
	
	/** @brief The current generation of Genomes - these are first tested, then a new generation is created afterwards to replace it */
	TArray<FGenomes*> CurrentGenerationGenomes;

	/** @brief TArray of the best available decoded Genomes, and all that came before it */
	TArray<TArray<float>> BestAvailableGenomesDecoded;
};
