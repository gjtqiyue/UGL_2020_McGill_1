// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

UCLASS()
class MCGILL_1_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetArmLength(TArray<AActor*> localPlayerRef, USpringArmComponent* springarm);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetCameraLocation(TArray<AActor*> localPlayerRef, float delTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float minLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float maxLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float interpSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector targetLocation;
	FVector currentLocation;
	FVector averageLocation;
	TArray<float> playerDistance;
	//USpringArmComponent* springarm = nullptr;
};
