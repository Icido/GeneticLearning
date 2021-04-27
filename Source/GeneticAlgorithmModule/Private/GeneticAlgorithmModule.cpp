// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneticAlgorithmModule.h"

DEFINE_LOG_CATEGORY(GeneticAlgorithmModule);

#define LOCTEXT_NAMESPACE "FGeneticAlgorithmModule"

void FGeneticAlgorithmModule::StartupModule()
{
	UE_LOG(GeneticAlgorithmModule, Warning, TEXT("GeneticAlgorithm Module has started!"));
}

void FGeneticAlgorithmModule::ShutdownModule()
{
	UE_LOG(GeneticAlgorithmModule, Warning, TEXT("GeneticAlgorithm Module has shut down!"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGeneticAlgorithmModule, GeneticAlgorithmModule);
