// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerationNode.h"
#include "Engine/World.h"
#include "GenerationManager.generated.h"

UCLASS(BlueprintType)
class MCGILL_1_API AGenerationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerationManager();
	void Generate(UGenerationNode& node);
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> item;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> floor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
