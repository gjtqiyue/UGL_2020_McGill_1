// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EEnemyType : uint8
{
	EPullToy 				UMETA(DisplayName = "Pull Toy"),
	ESockMonkey 			UMETA(DisplayName = "Sock Monkey"),
	ENoShield				UMETA(DisplayName = "Pull Toy No Shield"),
	EOneKill				UMETA(DisplayName = "Pull Toy One Hit"),
	EGiantBear				UMETA(DisplayName = "Giant Bear")
};

UCLASS()
class MCGILL_1_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, category = "Skill")
	void Attack();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, category = "Death")
	void Die();

	UFUNCTION(BlueprintCallable)
	void Pause();

	UFUNCTION(BlueprintCallable)
	void Resume();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Deactivate();

	UFUNCTION(BlueprintCallable)
	void KnockedBack(FVector hitPosition, FVector dir, float amt);

	UFUNCTION(BlueprintCallable)
	void AddVelocity(FVector vel);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EEnemyType enemyType;
	bool onGround;

	UPROPERTY(BlueprintReadWrite)
	FVector velocity;
	

private:
};
