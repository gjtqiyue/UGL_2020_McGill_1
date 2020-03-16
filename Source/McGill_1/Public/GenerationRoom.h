// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"
#include "GenerationRoom.generated.h"

#define WALL_SYM 1
#define OUTTER_SYM 2
#define INNER_SYM 0

USTRUCT(BlueprintType)
struct MCGILL_1_API FWall {
	GENERATED_BODY()
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector2D start;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector2D end;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool hasDoor = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int room1 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int room2 = 0;
};

USTRUCT(BlueprintType)
struct MCGILL_1_API FRoom {
	GENERATED_BODY()
public:
	TArray<FWall> walls;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TArray<FVector2D> corners;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int roomRep;
};

USTRUCT(BlueprintType)
struct MCGILL_1_API FGeneratedLevel {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TArray<FRoom> rooms;
};

UCLASS(BlueprintType)
class MCGILL_1_API ARoomGeneration : public AActor {
	GENERATED_BODY()
public:
	ARoomGeneration();
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int LevelWidth;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int LevelHeight;
	UFUNCTION(BlueprintCallable)
	FGeneratedLevel& GenerateRoomWalls(UPARAM(ref)  TArray<FVector2D>& corners, int numberOfRooms);
	UFUNCTION(BlueprintCallable)
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	int GetLevelElement(TArray<int>& level, float x, float y);
	UFUNCTION(BlueprintCallable)
    void SetLevelElement(TArray<int>& level, int x, int y, int value);
	
private:
	void GenerateLevelLayout(TArray<int>& level, TArray<FVector2D>& corners);
	void MoveHorizontal(TArray<int>& level, FVector2D& start, FVector2D& end);
	void MoveVertical(TArray<int>& level, FVector2D& start, FVector2D& end);
	void PrintLevel(TArray<int>& level);
	TArray<FRoom>& GenerateRandomRooms(TArray<int>& level, int numberRooms);
	bool TestPosition(TArray<int>& level, int x, int y);
	bool TestRect(TArray<int>& level, int x1, int y1, int x2, int y2);
	bool GrowRoom(FRoom& room, TArray<int>& level);
	void FindCorners(FRoom& room, TArray<int>& level);
	void AddCorner(TArray<FRoom>& rooms, FVector2D point, int roomId);
	void FindFirstRoomAppearance(int rep, TArray<int>& level, FVector2D& point);
};

