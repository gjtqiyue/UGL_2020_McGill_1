// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Character_Kid.generated.h"

/**
 * 
 */
UCLASS()
class MCGILL_1_API ACharacter_Kid : public ACharacterBase
{
	GENERATED_BODY()

public:
	ACharacter_Kid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
