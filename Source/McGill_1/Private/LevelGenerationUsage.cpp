// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerationUsage.h"

//Assume the pivot  of the wall is at the lower left corner
TArray<FTransform>& ULevelGenerationUsage::GetWallCreationTransforms(UPARAM(ref) FWall& wall, float scale) {
	TArray<FTransform>* transforms = new TArray<FTransform>();
	int zRotation = 0;
	int distance;
	int yDistance = abs((int)round(wall.end.Y - wall.start.Y));
	int xDistance = abs((int)round(wall.end.X - wall.start.X));
	FVector2D moveDirection;
	if (yDistance == 0) {
		distance = xDistance;
		float move = signbit(wall.end.X - wall.start.X)? -1 : 1;
		if (move == 1) {
			zRotation = 0;
		}
		else {
			zRotation = 180;
		}
		moveDirection = FVector2D(move, 0);
	}
	else if (xDistance == 0) {
		distance = yDistance;
		float move = signbit(wall.end.Y - wall.start.Y)? -1 : 1;
		if (move == 1) {
			zRotation = 90;
		}
		else {
			zRotation = -90;
		}
		moveDirection = FVector2D(0, move);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Impossible for a wall to go diagonal"));
	}
	for (int i = 0; i < distance; i++) {
		FTransform transform = FTransform();

		// figure out the position
		FVector2D start = wall.start * scale + i * scale * moveDirection;
		FVector position = FVector(start.X, start.Y, 0);

		// set location (only rotate around the z axis)
		FRotator rotator = FRotator(0, zRotation, 0);
		//create new transform for the wall
		transform.SetLocation(position);
		transform.SetRotation(FQuat(rotator));
		transforms->Emplace(transform);
	}
	return *transforms;
}

TArray<FWall>& ULevelGenerationUsage::CreateLevelWallFromRooms(UPARAM(ref) TArray<FRoom>& rooms) {
	TArray<FWall>* walls = new TArray<FWall>();
	for (FRoom room : rooms) {
		for (int i = 0; i < room.corners.Num(); i++) {
			// potential wall candidates
			FVector2D start = room.corners[i];
			FVector2D end = room.corners[(i + 1) % room.corners.Num()];
			TArray<FWall> roomWalls = CreateWalls(start, end, room.roomRep);

			for (FWall roomWall : roomWalls) {
				bool shouldAdd = true;

				for (int j = 0; j < walls->Num(); j++) {
					// check if the wall, or an opposite wall has been added to the array
					FWall* wall = &(*walls)[j];
					if ((wall->start.Equals(roomWall.start) && wall->end.Equals(roomWall.end)) ||
						(wall->start.Equals(roomWall.end) && wall->end.Equals(roomWall.start))) {
						wall->room2 = room.roomRep;
						shouldAdd = false;
					}
				}

				if (shouldAdd) {
					walls->Emplace(roomWall);
				}
			}
		}
	}

	return *walls;
}

TArray<FWall>& ULevelGenerationUsage::CreateWalls(FVector2D& start, FVector2D& end, int roomRep) {
	TArray<FWall>* walls = new TArray<FWall>();
	FVector2D dir = (end - start);
	dir.Normalize();
	FVector2D currentStart = start;
	while (!currentStart.Equals(end)) {
		FWall newWall;
		newWall.start = currentStart;
		newWall.end = currentStart + dir;
		newWall.room1 = roomRep;
		walls->Add(newWall);
		currentStart += dir;
	}

	return *walls;
}

void ULevelGenerationUsage::GenerateDoorWalls(UPARAM(ref) TArray<FWall>& walls) {
	TSet<int> connectedRooms;
	TArray<FWall*> doorWalls;
	TQueue<FWall*> poolWalls;
	for (int i = 0; i < walls.Num(); i++) {
		//only add non-external walls
		if (walls[i].room1 != 0 && walls[i].room2 != 0) {
			poolWalls.Enqueue(&walls[i]);
		}
		walls[i].hasDoor = false;
	}
	
	// add initial room
	FWall* firstWall;
	poolWalls.Dequeue(firstWall);
	doorWalls.Add(firstWall);
	connectedRooms.Add(firstWall->room1);
	connectedRooms.Add(firstWall->room2);
	int guard = 0;
	// generate rooms using spanning trees
	while (!poolWalls.IsEmpty() && guard < 2000) {
		guard++;
		FWall* wall;
		if (!poolWalls.Dequeue(wall)) {\
			UE_LOG(LogTemp, Error, TEXT("Noooo"));
			break;
		}

		if ((connectedRooms.Contains(wall->room1) && !connectedRooms.Contains(wall->room2)) ||
			(connectedRooms.Contains(wall->room2) && !connectedRooms.Contains(wall->room1))) {
			doorWalls.Add(wall);
			// duplicated rooms will be eliminated
			connectedRooms.Add(wall->room1);
			connectedRooms.Add(wall->room2);
		}
		else if (connectedRooms.Contains(wall->room1) && connectedRooms.Contains(wall->room2)) {
			// already connected, discard
		}
		else {
			// both not in, add back
			poolWalls.Enqueue(wall);
		}
	}
	for (int i = 0; i < doorWalls.Num(); i++) {
		doorWalls[i]->hasDoor = true;
	}
}

