/// Fill out your copyright notice in the Description page of Project Settings.
#include "InGameState.h"

void AInGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// any variable that want to be replicated goes here
	DOREPLIFETIME(AInGameState, CollectedItemA);
}

void AInGameState::AddScore() {
	if (HasAuthority()) {
		CollectedItemA++;
	}
}

