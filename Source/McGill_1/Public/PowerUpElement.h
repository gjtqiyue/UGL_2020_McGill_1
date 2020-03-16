// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpElement.generated.h"

class APowerUpSpawner;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPowerUpType : uint8
{
	EDadMoveSpeedBoost				UMETA(DisplayName = "Dad Move Speed Boost"),
	EDadAttackSpeedBoost 			UMETA(DisplayName = "Dad Attack Speed Boost"),
	EGirlMoveSpeedBoost 			UMETA(DisplayName = "Girl Move Speed Boost"),
	EGirlInvincibilityBoost         UMETA(DisplayName = "Girl Invincibility Boost"),
	EDefault
};

UCLASS()
class MCGILL_1_API APowerUpElement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BluePrintCallable)
	void Spread();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	EPowerUpType type;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	UMaterial* material;

	UPROPERTY(BlueprintReadWrite)
	APowerUpSpawner* spawner;
};
