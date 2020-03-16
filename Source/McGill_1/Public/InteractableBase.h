// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

class ACharacterBase;

UCLASS()
class MCGILL_1_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, category = "Interaction")
	void Trigger();

	UFUNCTION(BlueprintCallable, category = "Interaction")
	virtual void OnPlayerInRange(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION(BlueprintCallable, category = "Interaction")
	virtual void OnPlayerLeaveRange(class AActor* OverlappedActor, class AActor* OtherActor);

protected:
	class UBoxComponent* TriggerBox;
	TArray<ACharacterBase*> PlayersInRange;
	bool bInInteractRange;
	
};
