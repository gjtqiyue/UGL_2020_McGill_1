// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "PowerUpElement.h"
#include "PowerUpSpawner.generated.h"

USTRUCT(BlueprintType)
struct FPowerUpSetting
{
	GENERATED_USTRUCT_BODY()

	public:
	FPowerUpSetting() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<APowerUpElement> spawnClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterial* representedMat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int maxLimit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int minLimit;
};

UCLASS()
class MCGILL_1_API APowerUpSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnPowerUp(FVector location);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpreadRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EPowerUpType, FPowerUpSetting> SpawnConfig;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool OneTypeOnly;
};
