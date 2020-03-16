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

TArray<FRoom>& ULevelGenerationUsage::DecomposeRoom(UPARAM(ref) FRoom& room) {
	TArray<FRoom>* rooms = new TArray<FRoom>();
	//check for reflect point
	int firstReflectPosition = -1;
	int oppositeReflectPosition = -1;
	int cornerLength = room.corners.Num();
	for (int i = 0; i < cornerLength; i++) {
		if (IsReflexPoint(room.corners, i)) {
			firstReflectPosition = i;
			break;
		}
	}

	// already satisfies condition, return
	if (firstReflectPosition == -1) {
		rooms->Add(room);
		return *rooms;
	}

	// find the other reflex point that is opposite to this one (if any)
	for (int i = 0; i < cornerLength; i++) {
		//skip the one we already found
		if (i == firstReflectPosition) {
			continue;
		}
		else if (IsReflexPoint(room.corners, i) && (room.corners[i].X == room.corners[firstReflectPosition].X || room.corners[i].Y == room.corners[firstReflectPosition].Y)) {
			oppositeReflectPosition = i;
			break;
		}
	}

	// if there is an opposite reflex point
	if (oppositeReflectPosition != -1) {
		FRoom room1;
		room1.roomRep = room.roomRep;
		for (int i = (firstReflectPosition + 1) % cornerLength; i != oppositeReflectPosition; i = (i + 1) % cornerLength) {
			room1.corners.Add(room.corners[i]);
		}
		FRoom room2;
		room2.roomRep = room.roomRep;

		for (int i = (oppositeReflectPosition + 1) % cornerLength; i != firstReflectPosition; i = (i + 1) % cornerLength) {
			room2.corners.Add(room.corners[i]);
		}

		// put the two extra point to the necessary half
		AddToRoom(room1, room2, room.corners[firstReflectPosition], room.corners[oppositeReflectPosition]);


		rooms->Append(DecomposeRoom(room1));
		rooms->Append(DecomposeRoom(room2));
	}
	else {
		// only one reflect point, create a horizontal / vertical line
		int minLength = INT_MAX;
		FVector2D candidatePoint;
		FVector2D& reflex = room.corners[firstReflectPosition];
		for (int i = 0; i < cornerLength; i++) {
			FVector2D& a = room.corners[i];
			FVector2D& b = room.corners[(i + 1) % cornerLength];
			if (((a.X < reflex.X && b.X > reflex.X) || (a.X > reflex.X && b.X < reflex.X)) && a.Y != reflex.Y) {
				int length = std::abs(a.Y - reflex.Y);
				if (length < minLength) {
					minLength = length;
					oppositeReflectPosition = i;
					candidatePoint.X = reflex.X;
					candidatePoint.Y = a.Y;
				}
			}
			else if(((a.Y < reflex.Y && b.Y> reflex.Y) || (a.Y > reflex.Y && b.Y < reflex.Y)) && a.X != reflex.X){
				int length = std::abs(a.X - reflex.X);
				if (length < minLength) {
					minLength = length;
					oppositeReflectPosition = i;
					candidatePoint.X = a.X;
					candidatePoint.Y = reflex.Y;
				}
			}
		}

		if (oppositeReflectPosition == -1) {
			UE_LOG(LogTemp, Error, TEXT("Noooo"));
			return *rooms;
		}

		FRoom room1;
		room1.roomRep = room.roomRep;

		// want ton include the opposite one this time
		for (int i = (firstReflectPosition + 1) % cornerLength; i != (oppositeReflectPosition + 1) % cornerLength; i = (i + 1) % cornerLength) {
			room1.corners.Add(room.corners[i]);
		}
		room1.corners.Add(candidatePoint);

		FRoom room2;
		room2.roomRep = room.roomRep;

		room2.corners.Add(candidatePoint);
		for (int i = (oppositeReflectPosition + 1) % cornerLength; i != firstReflectPosition; i = (i + 1) % cornerLength) {
			room2.corners.Add(room.corners[i]);
		}

		if (candidatePoint.X == room.corners[firstReflectPosition].X) {
			if (room1.corners[0].X == candidatePoint.X) {
				room2.corners.Add(room.corners[firstReflectPosition]);
			}
			else {
				room1.corners.Add(room.corners[firstReflectPosition]);
			}
		}
		else {
			if (room1.corners[0].Y == candidatePoint.Y) {
				room2.corners.Add(room.corners[firstReflectPosition]);
			}
			else {
				room1.corners.Add(room.corners[firstReflectPosition]);
			}
		}

		rooms->Append(DecomposeRoom(room1));
		rooms->Append(DecomposeRoom(room2));
	}


	return *rooms;
}

bool ULevelGenerationUsage::IsReflexPoint(TArray<FVector2D>& corners, int index) {
	int cornerLength = corners.Num();
	int previous = index == 0 ? cornerLength - 1 : (index - 1) % cornerLength;
	FVector2D& a = corners[previous];
	FVector2D& b = corners[index];
	FVector2D& c = corners[(index + 1) % cornerLength];
	FVector2D line1 = c - b;
	FVector2D line2 = a - b;
	line1.Normalize();
	line2.Normalize();
	if (FVector2D::CrossProduct(line1, line2) < 0) {
		return true;
	}
	return false;
}

void ULevelGenerationUsage::AddToRoom(FRoom& room1, FRoom& room2, FVector2D& point1, FVector2D& point2) {
	if (point1.X == point2.X) {
		if (room1.corners[0].X == point1.X) {
			room2.corners.Insert(point1, 0);
			if (!point1.Equals(point2)) {
				room2.corners.Add(point2);
			}
		}
		else {
			room1.corners.Insert(point1, 0);
			if (!point1.Equals(point2)) {
				room1.corners.Add(point2);
			}
		}
	}
	else {
		if (room1.corners[0].Y == point1.Y) {
			room2.corners.Insert(point1, 0);
			
			if (!point1.Equals(point2)) {
				room2.corners.Add(point2);
			}
		}
		else {
			room1.corners.Insert(point1, 0);
			
			if (!point1.Equals(point2)) {
				room1.corners.Add(point2);
			}
		}
	}
}