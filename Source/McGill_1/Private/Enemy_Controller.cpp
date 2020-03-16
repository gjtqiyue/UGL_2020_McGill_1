// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



AEnemy_Controller::AEnemy_Controller() {

	PrimaryActorTick.bCanEverTick = true;

	//set up the config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//if we have other sense like hearing, it will affect the sensing, so set the dominant sense to sight
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());

	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemy_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AEnemy_Controller::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("begin"));
	if (GetPerceptionComponent() == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Something is wrong with the perception system"));
	}
	if (GetPerceptionComponent() != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Perception is ready"));
	}
}

void AEnemy_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UE_LOG(LogTemp, Warning, TEXT("ai sight radius %f"), SightConfig->SightRadius);
	//UE_LOG(LogTemp, Warning, TEXT("actual sight radius %f"), AISightRadius);
	if (GetPawn()->GetDistanceTo(ChaseTarget) > AISightRadius)
	{
		bIsPlayerDetected = false;
		ChaseTarget = nullptr;
	}
}

FRotator AEnemy_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr) {
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AEnemy_Controller::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	//TODO: if detected the player with higher priority, set the target to that player
	if(ChaseTarget != nullptr) return;

	ACharacterBase* player = nullptr;
	//check detected pawns if anyone of them is player
	for (size_t i = 0; i < DetectedPawns.Num(); ++i) {
		player = Cast<ACharacterBase>(DetectedPawns[i]);
		if (player != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Player detected"));
			if (player->CharacterRole == PriorityTarget) {
				
				bIsPlayerDetected = true;
				ChaseTarget = Cast<ACharacterBase>(player);
				DistanceToTarget = GetPawn()->GetDistanceTo(ChaseTarget);
				UE_LOG(LogTemp, Warning, TEXT("Target match the priority. distance is %f"), DistanceToTarget);
				return;
			}
		}
	}

	if(player != nullptr)
	{
		bIsPlayerDetected = true;
		ChaseTarget = nullptr;
		ChaseTarget = Cast<ACharacterBase>(player);
		DistanceToTarget = GetPawn()->GetDistanceTo(ChaseTarget);
		UE_LOG(LogTemp, Warning, TEXT("Distance to target is %f"), DistanceToTarget);

		return;
	}
}
