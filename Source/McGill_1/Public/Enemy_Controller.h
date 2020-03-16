// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "Enemy_Controller.generated.h"

/**
 * 
 */
UCLASS()
class MCGILL_1_API AEnemy_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_Controller();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
	virtual void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	UFUNCTION(BlueprintCallable, category="Enemy") 
	bool IsPlayerDetected() const
	{
		return bIsPlayerDetected;
	}

	UFUNCTION(BlueprintCallable, category = "Enemy")
	ACharacterBase* GetTarget() const
	{
		return ChaseTarget;
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AISightRadius = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = 1000 + 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AISightAge = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 180.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerRole PriorityTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MoveSpeed = 2.5f;

protected:
	bool bIsPlayerDetected = false;

	UPROPERTY(BlueprintReadWrite)
	ACharacterBase* ChaseTarget = nullptr;
	UPROPERTY(BlueprintReadWrite)
	ACharacterBase* FirstTarget = nullptr;

	float DistanceToTarget;
	
};
