// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameState.generated.h"

/**
 * 
 */
UCLASS()
class MCGILL_1_API AInGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
		int CollectedItemA;

	void AddScore();
};
