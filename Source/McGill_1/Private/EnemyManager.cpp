// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	//FFastXml::ParseXmlFile();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::AddRandomFormations(int num)
{
	if(!randomSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please turn on the random spawn switch to spawn randomly"));
	}

	for(int i = 0; i < num; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("adding formation......"));
		AddRandomFormation();
	}
}

void AEnemyManager::AddRandomFormation()
{
	FEnemyFormationConfig formation;
	formation.center = FVector::ZeroVector;

	int pullToyNum, monkeyNum, bearNum, tempt = 0;
	do
	{
		pullToyNum = FMath::FRandRange(randomEnemyFormationPullToyMin, randomEnemyFormationPullToyMax);
		monkeyNum = FMath::FRandRange(randomEnemyFormationSockMonkeyMin, randomEnemyFormationSockMonkeyMax);
		bearNum = FMath::FRandRange(randomEnemyFormationGiantBearMin, randomEnemyFormationGiantBearMax);
		tempt++;
	} while(pullToyNum + monkeyNum + bearNum > randomEnemyFormationNum && tempt < 10);

	for(int i = 0; i < pullToyNum; i++)
	{
		FRotator rotation;
		rotation = FRotator::MakeFromEuler(FVector(0.f, 0.f, FMath::RandRange(-179, 180)));
		FVector position = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(1., randomEnemyFormationMaxOffset);
		position.Z = 0;
		FEnemyFormationNode node;
		node.type = EEnemyType::EPullToy;
		node.location = position;
		node.rotation = rotation;
		formation.members.Add(node);
	}

	for(int i = 0; i < monkeyNum; i++)
	{
		FRotator rotation;
		rotation = FRotator::MakeFromEuler(FVector(0.f, 0.f, FMath::RandRange(-179, 180)));
		FVector position = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(1., randomEnemyFormationMaxOffset);
		position.Z = 0;
		FEnemyFormationNode node;
		node.type = EEnemyType::ESockMonkey;
		node.location = position;
		node.rotation = rotation;
		formation.members.Add(node);
	}

	for (int i = 0; i < bearNum; i++)
	{
		FRotator rotation;
		rotation = FRotator::MakeFromEuler(FVector(0.f, 0.f, FMath::RandRange(-179, 180)));
		FVector position = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(1., randomEnemyFormationMaxOffset);
		position.Z = 0;
		FEnemyFormationNode node;
		node.type = EEnemyType::EGiantBear;
		node.location = position;
		node.rotation = rotation;
		formation.members.Add(node);
	}

	FString name = FString("Random_");
	name.AppendInt(formationList.Num());
	UE_LOG(LogTemp, Warning, TEXT("Spawning %d toys and %d monkeys, I have %d formations now"), pullToyNum, monkeyNum, formationList.Num());
	AddFormation(name, formation);
}

void AEnemyManager::AddFormation(FString key, FEnemyFormationConfig formation)
{
	if (!formationList.Contains(key)) {
		formationList.Add(key, formation);
	}
	else {
		formationList[key] = formation;
	}
}

void AEnemyManager::RemoveFormation(FString key)
{
	if (formationList.Contains(key)) {
		formationList.Remove(key);
	}
}

bool AEnemyManager::SpawnFormation(FString formation_key, FVector location, int id)
{
	//TODO: add later if id is > 0 then use the room location as the default location
	if (PullToyClass == nullptr || SockMonkeyClass == nullptr || PlayerClass == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No class assigned for sub classes in enemy manager (Player, PullToy, SockMonkey)"));
		return false;
	}
	TArray<AEnemyBase*> list;
	FEnemyFormationConfig formation = formationList[formation_key];

	if (!enemyList.Contains(id)) {
		enemyList.Add(id, list);
	}
	//formation.center = location;
	AEnemyBase* enemy;
	for (auto& elem : formation.members) {
		switch (elem.type) {
		case EEnemyType::EPullToy:
			//spawn a pull toy at the destination
			enemy = GetWorld()->SpawnActor<AEnemyBase>(PullToyClass->GetAuthoritativeClass(), location + elem.location, elem.rotation);
			enemyList[id].Add(enemy);
			break;
		case EEnemyType::ESockMonkey:
			//spawn a socky monkey at the destination
			enemy = GetWorld()->SpawnActor<AEnemyBase>(SockMonkeyClass->GetAuthoritativeClass(), location + elem.location, elem.rotation);
			enemyList[id].Add(enemy);
			break;
		case EEnemyType::ENoShield:
			//spawn a socky monkey at the destination
			enemy = GetWorld()->SpawnActor<AEnemyBase>(PullToyNoShieldClass->GetAuthoritativeClass(), location + elem.location, elem.rotation);
			enemyList[id].Add(enemy);
			break;
		case EEnemyType::EOneKill:
			//spawn a socky monkey at the destination
			enemy = GetWorld()->SpawnActor<AEnemyBase>(PullToyOneKillClass->GetAuthoritativeClass(), location + elem.location, elem.rotation);
			enemyList[id].Add(enemy);
			break;
		case EEnemyType::EGiantBear:
			//spawn a socky monkey at the destination
			enemy = GetWorld()->SpawnActor<AEnemyBase>(BearClass->GetAuthoritativeClass(), location + elem.location, elem.rotation);
			enemyList[id].Add(enemy);
			break;
		}
	}
	return true;
}

bool AEnemyManager::SpawnRandomFormation(int room_id, FVector location)
{
	UE_LOG(LogTemp, Warning, TEXT("spawning random formation......"));
	TArray<FString> array;
	formationList.GenerateKeyArray(array);
	int keyIndex = (int)FMath::FRandRange(0, array.Num()-1);
	
	//TODO: get a random location from the nav mesh, and assign to the formation struct

	return SpawnFormation(array[keyIndex], location, room_id);
}

void AEnemyManager::DeathReport(int id, AEnemyBase * deadEnemy)
{
	enemyList[id].Remove(deadEnemy);
}

bool AEnemyManager::Pause()
{
	if (activeLevel >= 0) {
		//pause the room
		for (AEnemyBase* enemy : enemyList[activeLevel]) {
			pauseStatus = 1;
			enemy->Pause();
		}
		return true;
	}
	else if (dadLevel >= 0 && childLevel >= 0) {
		pauseStatus = 2;
		for (AEnemyBase* enemy : enemyList[dadLevel]) {
			enemy->Pause();
		}
		for (AEnemyBase* enemy : enemyList[childLevel]) {
			enemy->Pause();
		}
		return true;
	}
	return false;
}

bool AEnemyManager::Resume()
{
	if (activeLevel >= 0) {
		//pause the room
		for (AEnemyBase* enemy : enemyList[activeLevel]) {
			pauseStatus = 1;
			enemy->Resume();
		}
		return true;
	}
	else if (dadLevel >= 0 && childLevel >= 0) {
		pauseStatus = 2;
		for (AEnemyBase* enemy : enemyList[dadLevel]) {
			enemy->Resume();
		}
		for (AEnemyBase* enemy : enemyList[childLevel]) {
			enemy->Resume();
		}
		return true;
	}
	return false;
}

bool AEnemyManager::Activate(int room_id)
{
	if (enemyList[room_id].Num() == 0) {
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("deactivating %d enemies at room %d"), enemyList[room_id].Num(), room_id);
	for (AEnemyBase* enemy : enemyList[room_id]) {
		enemy->Activate();
	}
	return true;
}

bool AEnemyManager::Deactivate(int room_id)
{
	if (enemyList[room_id].Num() == 0) {
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("deactivating %d enemies at room %d"), enemyList[room_id].Num(), room_id);
	for (AEnemyBase* enemy : enemyList[room_id]) {
		enemy->Deactivate();
	}
	return true;
}

void AEnemyManager::ClearEnemiesInRoom(int room_id)
{
	if (!enemyList.Contains(room_id)) {
		return;
	}
	if (enemyList[room_id].Num() == 0) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("clearing %d enemies"), enemyList[room_id].Num());
	for (AEnemyBase* enemy : enemyList[room_id]) {
		enemy->Die();
	}

	enemyList[room_id].Empty();
}

//bool AEnemyManager::UpdateDustBunnyCount(int player_id) {
//	if (player_id != 0 && player_id != 1) return false;  
//	bunniesCount[player_id]--;
//	if (bunniesCount[player_id] <= 0) {
//		return true;
//	}
//	return false;
//}

