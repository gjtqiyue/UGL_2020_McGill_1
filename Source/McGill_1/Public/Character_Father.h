// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Character_Father.generated.h"

/**
 * 
 */
UCLASS()
class MCGILL_1_API ACharacter_Father : public ACharacterBase
{
	GENERATED_BODY()

public:
	ACharacter_Father();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
