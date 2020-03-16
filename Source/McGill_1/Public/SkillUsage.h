// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CharacterBase.h"
#include "SkillInfo.h"
#include "SkillUsage.generated.h"
/**
 * 
 */
UCLASS()
class MCGILL_1_API USkillUsage : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void Fire(ACharacterBase* character, FString skillname, FTransform spawnParent);

	UFUNCTION(BlueprintCallable)
	static void ActivatePowerUp(ACharacterBase* character, EPowerUpType name);

	UFUNCTION(BlueprintCallable)
	static void Cooldown(float deltaTime, struct FSkillInfo& skill);

private:
	static void FireProjectile(ACharacterBase* character, struct FSkillInfo& skill, FTransform spawnParent);
};
