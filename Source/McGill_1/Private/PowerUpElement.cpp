// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpElement.h"
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"

// Sets default values
APowerUpElement::APowerUpElement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void APowerUpElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUpElement::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerUpElement, material);
	DOREPLIFETIME(APowerUpElement, type);
}

