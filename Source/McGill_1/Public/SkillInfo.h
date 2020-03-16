// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "SkillInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESkillType : uint8 {
	VE_Melee	UMETA(DisplayName = "Melee"),
	VE_Projectile UMETA(DisplayName = "Projectile")
};

USTRUCT(BlueprintType)
struct MCGILL_1_API FSkillInfo {
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere)
	FString SkillName;
	UPROPERTY(EditAnyWhere)
	float Timer;

	UPROPERTY(EditAnyWhere)
	float Cooldown;
	UPROPERTY(EditAnyWhere)
	float Speed;
	UPROPERTY(EditAnyWhere)
	float Range;
	UPROPERTY(EditAnyWhere)
	float Damage;

	UPROPERTY(EditAnyWhere)
	ESkillType type;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<AProjectileBase> projectile;
};
