// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerTestObject.h"
AMultiplayerTestObject::AMultiplayerTestObject()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMultiplayerTestObject::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMultiplayerTestObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if (HasAuthority()) {
		FVector Location = GetActorLocation();
		Location += FVector(Speed * DeltaTime, 0, 0);
		SetActorLocation(Location);
		
	}

}
