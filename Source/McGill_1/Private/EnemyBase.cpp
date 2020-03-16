// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//update velocity if it is no 0
	//if (onGround) {
	//	velocity = FVector::ZeroVector;
	//}

	if (!velocity.IsZero()) {
		SetActorLocation(GetActorLocation() + velocity * DeltaTime);
		//velocity.Y -= 10 * DeltaTime;
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemyBase::KnockedBack(FVector hitPosition, FVector dir, float amt)
{
	AddVelocity(dir * amt);
}

void AEnemyBase::AddVelocity(FVector vel)
{
	velocity += vel;
}

void AEnemyBase::Pause()
{
}

void AEnemyBase::Resume()
{
}

