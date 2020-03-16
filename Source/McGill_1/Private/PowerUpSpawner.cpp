// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUpSpawner.h"
#include "Engine/World.h"


// Sets default values
APowerUpSpawner::APowerUpSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerUpSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUpSpawner::SpawnPowerUp(FVector location)
{
	APowerUpElement* object = nullptr;
	//spawn only one kind in random
	UE_LOG(LogTemp, Warning, TEXT("spawn power up"));
	if (OneTypeOnly) {
		int selectNum = FMath::FRandRange(0, SpawnConfig.Num());
		UE_LOG(LogTemp, Warning, TEXT("select %d"), selectNum);
		int count = 0;
		for (auto& Elem : SpawnConfig) {
			if (count == selectNum) {
				int spawnNum = FMath::FRandRange(Elem.Value.minLimit, Elem.Value.maxLimit);
				for (int i = 0; i < spawnNum; i++)
				{
					object = GetWorld()->SpawnActor<APowerUpElement>(Elem.Value.spawnClass->GetAuthoritativeClass(), location, FRotator::ZeroRotator);
					if (object != nullptr)
					{
						object->type = Elem.Key;
						object->material = Elem.Value.representedMat;
						object->spawner = this;
						object->Spread();
					}
				}
			}
			count++;
		}
	}
	// spawn everything
	else {
		for (auto& Elem : SpawnConfig)
		{
			int spawnNum = FMath::FRandRange(Elem.Value.minLimit, Elem.Value.maxLimit);
			for (int i = 0; i < spawnNum; i++)
			{
				object = GetWorld()->SpawnActor<APowerUpElement>(Elem.Value.spawnClass->GetAuthoritativeClass(), location, FRotator::ZeroRotator);
				if (object != nullptr)
				{
					object->type = Elem.Key;
					object->material = Elem.Value.representedMat;
					object->spawner = this;
					object->Spread();
				}
			}
		}
	}
}
