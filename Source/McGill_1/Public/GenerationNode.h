// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc\FileHelper.h"
#include "Misc/Paths.h"
#include "Internationalization/Regex.h"
#include "GenerationNode.generated.h"

/**
 * 
 */
struct GenerationProperty {
	float min;
	float max;
};


UCLASS()
class MCGILL_1_API UGenerationNode : public UObject
{
	GENERATED_BODY()

public:
	UGenerationNode();
	static UGenerationNode* Create(FString name, TSubclassOf<AActor>* actorClass);
	FString Name;
	TSubclassOf<AActor>* ActorClass;
	TArray<TArray<UGenerationNode*>> children;
	TArray<GenerationProperty> properties;
};

void ParseGenerationGrammar(FString fileName);
