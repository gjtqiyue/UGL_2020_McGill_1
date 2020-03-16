// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "CharacterBase.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TriggerBox = FindComponentByClass<class UBoxComponent>();
	//TriggerBox->BeginComponentOverlap.AddDynamic(this, &AInteractableBase::OnPlayerInRange);
	//TriggerBox->EndComponentOverlap.AddDynamic(this, &AInteractableBase::OnPlayerLeaveRange);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	PlayersInRange.Empty();
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//When player is in a interactable range, register the object pointer to player
void AInteractableBase::OnPlayerInRange(AActor * OverlappedActor, AActor * OtherActor)
{
	ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
	if (Player != nullptr) {
		PlayersInRange.Push(Player);
		Player->RegisterInteractableObject(this);
		UE_LOG(LogTemp, Warning, TEXT("Player in interact range of object"));
	}
}

//When player leaves
void AInteractableBase::OnPlayerLeaveRange(AActor * OverlappedActor, AActor * OtherActor)
{
	ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
	if (PlayersInRange.Num() > 0 && PlayersInRange.Contains(Player)) {
		Player->UnRegisterInteractableObject(this);
		PlayersInRange.Remove(Player);
		UE_LOG(LogTemp, Warning, TEXT("Player leave the range"));
	}
}

