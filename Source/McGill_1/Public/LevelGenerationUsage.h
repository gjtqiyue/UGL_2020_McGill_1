// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerationRoom.h"
#include "Containers/Queue.h"
#include <math.h>
#include "LevelGenerationUsage.generated.h"

/**
 * 
 */
UCLASS()
class MCGILL_1_API ULevelGenerationUsage : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<FWall>& CreateLevelWallFromRooms(UPARAM(ref) TArray<FRoom>& rooms);
	UFUNCTION(BlueprintCallable)
	static TArray<FTransform>& GetWallCreationTransforms(UPARAM(ref) FWall& wall, float scale);
	UFUNCTION(BlueprintCallable)
	static void GenerateDoorWalls(UPARAM(ref) TArray<FWall>& walls);
private:
	static TArray<FWall>& CreateWalls(FVector2D& start, FVector2D& end, int roomRep);
};
