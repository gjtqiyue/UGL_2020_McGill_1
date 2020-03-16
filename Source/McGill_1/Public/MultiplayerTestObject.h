// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MultiplayerTestObject.generated.h"

/**
 * 
 */
UCLASS()
class MCGILL_1_API AMultiplayerTestObject : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMultiplayerTestObject();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	float Speed = 20;
};
