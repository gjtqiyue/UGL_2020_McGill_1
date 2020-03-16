// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/Tuple.h"
#include "EnemyBase.h"
#include "CharacterBase.h"
#include "Math/NumericLimits.h"
#include "EnemyManager.generated.h"

USTRUCT(BlueprintType)
struct FEnemyFormationNode
{
	GENERATED_USTRUCT_BODY()

public:
	FEnemyFormationNode() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEnemyType type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector location;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator rotation;
};

USTRUCT(BlueprintType) 
struct FEnemyFormationConfig 
{
	GENERATED_USTRUCT_BODY()

public:
	FEnemyFormationConfig() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector center;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector facingDir;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FEnemyFormationNode> members;
};

UCLASS()
class MCGILL_1_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddRandomFormations(int num);

	void AddRandomFormation();

	UFUNCTION(BlueprintCallable)
	void AddFormation(FString key, FEnemyFormationConfig formation);

	UFUNCTION(BlueprintCallable)
	void RemoveFormation(FString key);

	UFUNCTION(BlueprintCallable)
	bool SpawnFormation(FString formation_key, FVector location, int id);

	UFUNCTION(BlueprintCallable)
	bool SpawnRandomFormation(int room_id, FVector location);

	UFUNCTION(BlueprintCallable)
	void DeathReport(int id, AEnemyBase* deadEnemy);

	UFUNCTION(BlueprintCallable)
	bool Pause();

	UFUNCTION(BlueprintCallable)
	bool Resume();

	UFUNCTION(BlueprintCallable)
	bool Activate(int room_id);

	UFUNCTION(BlueprintCallable)
	bool Deactivate(int room_id);

	UFUNCTION(BlueprintCallable)
	void ClearEnemiesInRoom(int room_id);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool UpdateDustBunnyCount(int player_id);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnBunnyGroup(ACharacterBase* player, int num);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnBunnyGroupAtLocation(FVector location, int num);

	UFUNCTION(BlueprintCallable)
	TArray<AEnemyBase*> GetEnemyListInRoom(int room_id) {
		if (enemyList.Contains(room_id)) {
			return enemyList[room_id];
		}
		else {
			TArray<AEnemyBase*> list;
			enemyList.Add(room_id, list);
			return list;
		}
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int activeLevel = -1;	//value is changed to the ID corresponds to the room that both players are in
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int dadLevel = -1;		//room ID that only dad is in
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int childLevel = -1;		//room ID that only girl is in

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Dust Bunnies")
	int dustBunniesSpawnLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Dust Bunnies")
	float dustBunniesSlowDownRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	bool randomSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	float randomEnemyFormationMaxOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationPullToyMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationSockMonkeyMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationGiantBearMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationPullToyMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationSockMonkeyMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Random Spawning")
	int randomEnemyFormationGiantBearMin;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyBase> PullToyClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyBase> PullToyNoShieldClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyBase> PullToyOneKillClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyBase> SockMonkeyClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyBase> BearClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACharacterBase> PlayerClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> DustBunnyClass;

	UPROPERTY(EditAnywhere)
	TMap<FString, FEnemyFormationConfig> formationList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Dust Bunnies")
	bool spawnDustBunnies;
	
	TMap<int, TArray<AEnemyBase*>> enemyList;

	UPROPERTY(VisibleAnywhere)
	int pauseStatus = 0;	// 0 is ok, 1 is same level, 2 is separate level
};
