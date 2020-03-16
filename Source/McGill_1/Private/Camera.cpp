// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACamera::SetArmLength(TArray<AActor*> localPlayerRef, USpringArmComponent* springarm)
{
	playerDistance.Empty();
	//UE_LOG(LogTemp, Warning, TEXT("%d"), localPlayerRef.Num());
	for (int i = 0; i < localPlayerRef.Num() - 1; ++i) {
		for (int j = i; j < localPlayerRef.Num(); ++j) {
			playerDistance.Add(localPlayerRef[i]->GetDistanceTo(localPlayerRef[j]));
		}
	}

	//springarm = Cast<USpringArmComponent>(OwnerActor->GetComponentByClass(USpringArmComponent::StaticClass()));
	//set the max distance to the arm length
	float target = FMath::Clamp(FMath::Max<float>(playerDistance), minLength, maxLength);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), target);
	springarm->TargetArmLength = target;
}

void ACamera::SetCameraLocation(TArray<AActor*> localPlayerRef, float delTime)
{
	averageLocation = FVector::ZeroVector;
	for (int i = 0; i < localPlayerRef.Num(); ++i) {
		averageLocation += localPlayerRef[i]->GetActorLocation();
	}
	//get average location of all player
	averageLocation /= localPlayerRef.Num();
	targetLocation = averageLocation + FVector(0, 0, 200);
	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), targetLocation.X, targetLocation.Y, targetLocation.Z);
	////lerp the location to the target location
	SetActorLocation(FMath::VInterpTo(currentLocation, targetLocation, delTime, interpSpeed));
	currentLocation = GetActorLocation();
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();

	//springarm = OwnerActor->FindComponentByClass<USpringArmComponent>();
	//if (springarm == nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("No SpringArm component found"));
	//	return;
	//}
}


