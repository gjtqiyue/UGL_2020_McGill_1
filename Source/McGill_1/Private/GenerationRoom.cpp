// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerationRoom.h"

ARoomGeneration::ARoomGeneration()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FGeneratedLevel& ARoomGeneration::GenerateRoomWalls(UPARAM(ref) TArray<FVector2D>& corners, int numberOfRooms) {
	FGeneratedLevel* levelGenerated =new FGeneratedLevel();
	TArray<int> level;
	GenerateLevelLayout(level, corners);
	TArray<FRoom>& rooms = GenerateRandomRooms(level, numberOfRooms);
	for (int i = 0; i < rooms.Num(); i++) {
		FindCorners(rooms[i], level);
		rooms[i].corners.RemoveAt(rooms[i].corners.Num() - 1);
	}
	levelGenerated->rooms = rooms;
	this->PrintLevel(level);

	return *levelGenerated;
}

void ARoomGeneration::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomGeneration::GenerateLevelLayout(TArray<int>& level, TArray<FVector2D>& corners) {
	int maxX = 0;
	int maxY = 0;

	// find dimension
	for (FVector2D corner : corners) {
		if (corner.X > maxX) {
			maxX = corner.X;
		}

		if (corner.Y > maxY) {
			maxY = corner.Y;
		}
	}
	this->LevelHeight = maxY + 1;
	this->LevelWidth = maxX + 1;
	// initialize everything to zero
	level.Init(OUTTER_SYM, (this->LevelHeight) * (this->LevelWidth));

	// create the outline
	for (int i = 0; i < corners.Num(); i++) {
		int next = (i == corners.Num() - 1) ? 0 : i + 1;
		if (corners[i].X < corners[next].X) {
			// move to the right
			this->MoveHorizontal(level, corners[i], corners[next]);
		}
		else if (corners[i].X > corners[next].X) {
			// move to the left
			this->MoveHorizontal(level, corners[next], corners[i]);
		}
		else if (corners[i].Y < corners[next].Y) {
			// move up
			this->MoveVertical(level, corners[i], corners[next]);
		}
		else {
			//move down
			this->MoveVertical(level, corners[next], corners[i]);
		}
	}

	// fill up the usable place
	for (int i = 0; i < this->LevelHeight; i++) {
		bool toggle1 = false;
		int previous = OUTTER_SYM;
		for (int j = 0; j < this->LevelWidth; j++) {
			int element = this->GetLevelElement(level, j, i);
			//enter the usable region
			if (element == 1 && previous == OUTTER_SYM) {
				toggle1 = !toggle1;
			}
			
			// the last row should not have any inner
			if (toggle1 && element == OUTTER_SYM && i != this->LevelHeight-1) {
				SetLevelElement(level, j, i, INNER_SYM);
			}
			previous = element;
		}
	}
}

void ARoomGeneration::MoveHorizontal(TArray<int>& level, FVector2D& start, FVector2D& end) {
	for (int i = start.X; i <= end.X; i++) {
		SetLevelElement(level, i, start.Y, WALL_SYM);
	}
}

void ARoomGeneration::MoveVertical(TArray<int>& level, FVector2D& start, FVector2D& end) {
	for (int i = start.Y; i <= end.Y; i++) {
		SetLevelElement(level, start.X, i, WALL_SYM);
	}
}

int ARoomGeneration::GetLevelElement(TArray<int>& level, float x, float y) {
	int x_int = (int)(x + 0.5);
	int y_int = (int)(y + 0.5);
	int index = x_int + y_int * this->LevelWidth;
	if (index < 0 || index >= this->LevelHeight * this->LevelWidth) {
		return -1;
	}
	return level[index];
}

void ARoomGeneration::SetLevelElement(TArray<int>& level, int x, int y, int value) {
	level[x + y * this->LevelWidth] = value;
}

void ARoomGeneration::PrintLevel(TArray<int>& level) {
	for (int i = 0; i < this->LevelHeight; i++) {
		FString content = "";
		for (int j = 0; j < this->LevelWidth; j++) {
			int element = this->GetLevelElement(level, j, i);
			content += FString::Printf(TEXT(" %d"), element);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *content);
	}
}

TArray<FRoom>& ARoomGeneration::GenerateRandomRooms(TArray<int>& level, int numberRooms) {
	TArray<FRoom>* roomsPointer = new TArray<FRoom>();
	TArray<FRoom>& rooms = *roomsPointer;

	rooms.Init(FRoom(), numberRooms);
	//pick random room starting positions
	for (int i = 0; i < numberRooms; i++) {
		rooms[i].roomRep = OUTTER_SYM + i + 1;
		int x, y;
		do {
			x = rand() % this->LevelWidth;
			y = rand() % this->LevelHeight;
		} while (!TestRect(level, x, y, x+1, y+1));
		rooms[i].corners.Add(FVector2D(x, y));
		rooms[i].corners.Add(FVector2D(x+1, y));
		rooms[i].corners.Add(FVector2D(x+1, y+1));
		rooms[i].corners.Add(FVector2D(x, y+1));
		//UE_LOG(LogTemp, Warning, TEXT("room: %f, %f"), rooms[i].corners[0].X, rooms[i].corners[0].Y);
		for (FVector2D corner : rooms[i].corners) {
			SetLevelElement(level, corner.X, corner.Y, rooms[i].roomRep);
		}
	}

	// get indexes of the room list to use for shuffle

	// Diagonal growth
	FVector2D dirs[] = { FVector2D(-1, -1), FVector2D(1, -1), FVector2D(1, 1), FVector2D(-1, 1) };
	TArray<FRoom*> cornerRooms;
	for (int i = 0; i < rooms.Num(); i++){
		cornerRooms.Add(&rooms[i]);
	}

	int i = 0;
	while (cornerRooms.Num() > 0) {
		// try all 4 directions
		bool didGrow = false;
		for (int d = 0; d < 4; d++) {
			int moveX = dirs[d].X;
			int moveY = dirs[d].Y;
			FVector2D& start = cornerRooms[i]->corners[d];
			int before = (d - 1) % 4;
			if (before < 0) before += 4;
			int after = (d + 1) % 4;
			FVector2D* horizontal = nullptr;
			FVector2D* vertical = nullptr;
			// find the horizontal and vertical based on the grow direction
			if (d == 1 || d == 3) {
				horizontal = &cornerRooms[i]->corners[after];
				vertical = &cornerRooms[i]->corners[before];
			}
			else {
				vertical = &cornerRooms[i]->corners[after];
				horizontal = &cornerRooms[i]->corners[before];
			}


			bool shouldGrow = true;
			//check vertical
			int nextX = horizontal->X + moveX;
			int nextY = vertical->Y + moveY;
			int largeY = horizontal->Y > nextY ? horizontal->Y : nextY;
			int smallY = horizontal->Y <= nextY ? horizontal->Y : nextY;

			for (int c = smallY; c <= largeY; c++) {
				if (GetLevelElement(level, nextX, c) != INNER_SYM) {
					shouldGrow = false;
					break;
				}
			}

			//UE_LOG(LogTemp, Warning, TEXT("smallY %d"), smallY);
			//UE_LOG(LogTemp, Warning, TEXT("largeY %d"), largeY);
			//check horizontal
			int largeX = nextX > vertical->X ? nextX : vertical->X;
			int smallX = nextX <= vertical->X ? nextX : vertical->X;
			for (int c = smallX; c <= largeX; c++) {
				if (GetLevelElement(level, c, nextY) != INNER_SYM) {
					shouldGrow = false;
					break;
				}
			}

			//if check failed, break
			if (!shouldGrow) {
				continue;
			}
			didGrow = true;
			// if pass set all the two lanes to this room
			for (int c = smallY; c <= largeY; c++) {
				SetLevelElement(level, nextX, c, cornerRooms[i]->roomRep);
			}
			for (int c = smallX; c <= largeX; c++) {
				SetLevelElement(level, c, nextY, cornerRooms[i]->roomRep);
			}
			// change corners
			start.X += moveX;
			start.Y += moveY;
			horizontal->X += moveX;
			vertical->Y += moveY;
			break;
		}

		// if no grow in any direction, remove that room
		if (!didGrow) {
			cornerRooms.RemoveAt(i);
			if (cornerRooms.Num() != 0) {
				// choose a random room
				int pos = FMath::RandRange(0, cornerRooms.Num() - 1);
				i = pos; // (i) % cornerRooms.Num();
			}
		}
		else {
			//go to next room
			if (cornerRooms.Num() != 0) {
				int pos = FMath::RandRange(0, cornerRooms.Num() - 1);
				i = pos; // (i + 1) % cornerRooms.Num();
			}
		}
	}


	//expand in L shape
	TArray<FRoom*> growableRooms;
	for (int c = 0; c < rooms.Num(); c++) {
		growableRooms.Add(&rooms[c]);
	}

	i = 0;
	while (growableRooms.Num() > 0) {
		if (!GrowRoom(*growableRooms[i], level)) {
			growableRooms.RemoveAt(i);
			if (growableRooms.Num() != 0) {
				int pos = FMath::RandRange(0, cornerRooms.Num() - 1);
				i = pos; //(i) % cornerRooms.Num();
			}
		}
		else {
			if (growableRooms.Num() != 0) {
				int pos = FMath::RandRange(0, cornerRooms.Num() - 1);
				i = pos; //(i + 1) % cornerRooms.Num();
			}
		}
	}
	
	return rooms;
}

bool ARoomGeneration::TestPosition(TArray<int>& level, int x, int y) {
	return GetLevelElement(level, x, y) == INNER_SYM;
}

bool ARoomGeneration::TestRect(TArray<int>& level, int x1, int y1, int x2, int y2) {
	return (TestPosition(level, x1, y1)) && (TestPosition(level, x1, y2)) &&
		(TestPosition(level, x2, y1)) && (TestPosition(level, x2, y2));
}

bool ARoomGeneration::GrowRoom(FRoom& room, TArray<int>& level){
	FVector2D dirs[] = { FVector2D(0, -1), FVector2D(1, 0), FVector2D(0, 1), FVector2D(-1, 0) };
	bool growed = false;
	for (int i = 0; i < 4; i++) {
		FVector2D& from = room.corners[i];
		int next = (i + 1) % 4;
		FVector2D& to = room.corners[next];
		from += dirs[i];
		to += dirs[i];

		// check the available space
		if (from.X == to.X) {
			int largerY = from.Y > to.Y ? from.Y : to.Y;
			int smallerY = from.Y <= to.Y ? from.Y : to.Y;
			for (int c = smallerY; c <= largerY; c++) {
				if(this->GetLevelElement(level, from.X, c) == INNER_SYM && this->GetLevelElement(level, from.X - dirs[i].X, c) == room.roomRep) {
					this->SetLevelElement(level, from.X, c, room.roomRep);
					growed = true;
				}
			}
		}
		else {
			int largerX = from.X > to.X ? from.X : to.X;
			int smallerX = from.X <= to.X ? from.X : to.X;
			for (int c = smallerX; c <= largerX; c++) {
				if(this->GetLevelElement(level, c, from.Y) == INNER_SYM && (this->GetLevelElement(level, c, from.Y - dirs[i].Y) == room.roomRep)) {
					this->SetLevelElement(level, c, from.Y, room.roomRep);
					growed = true;
				}
			}
		}

		if (growed) {
			break;
		}
		else {
			from -= dirs[i];
			to -= dirs[i];
		}
	}

	return growed;
}

void ARoomGeneration::AddCorner(TArray<FRoom>& rooms, FVector2D point, int roomId) {
	for (int i = 0; i < rooms.Num(); i++) {
		if (rooms[i].roomRep == roomId) {
			rooms[i].corners.Add(point);
			return;
		}
	}
}

void ARoomGeneration::FindCorners(FRoom& room, TArray<int>& level) {
	//first remove all previous walls and corners
	room.walls.Empty();
	room.corners.Empty();

	int rep = room.roomRep;
	//first find the first existence of the number
	FVector2D point;
	FindFirstRoomAppearance(rep, level, point);
	FVector2D corner = point + FVector2D(-1, -1);
	room.corners.Add(corner);
	//set corner to negative to wait for update
	FVector2D direction = FVector2D(1, 0);
	FVector2D startPoint = point;
	FVector2D pointToCheck;
	int counter = 0;
	do 
	{
		bool cornerUpdated = false;
		while (!cornerUpdated) {
			counter++;
			point += direction;
			// need to reverse y direction
			pointToCheck = point + direction.GetRotated(-90);
			// reach the end of the direction, should continue clockwise
			if (GetLevelElement(level, point.X, point.Y) != rep) {
				//UE_LOG(LogTemp, Warning, TEXT("convex p: %s"), *FString(point.ToString()));
				//UE_LOG(LogTemp, Warning, TEXT("convex n: %s"), *FString(pointToCheck.ToString()));
				// update the corner position
				corner = corner + (point - startPoint);
				room.corners.Add(corner);

				//start with previous position
				point -= direction;
				startPoint = point;
				// should continue clockwise
				direction = direction.GetRotated(90);
				cornerUpdated = true;
			}
			else if (GetLevelElement(level, pointToCheck.X, pointToCheck.Y) == rep) {
				//UE_LOG(LogTemp, Warning, TEXT("concave p: %s"), *FString(point.ToString()));
				//UE_LOG(LogTemp, Warning, TEXT("concave n: %s"), *FString(pointToCheck.ToString()));
				// meet the concave point, should continue anti-clockwise
				// update the corner position
				corner = corner + (point - startPoint);
				room.corners.Add(corner);

				// continue on current position
				// should continue counterclockwise
				direction = direction.GetRotated(-90);
				startPoint = point + direction;
				cornerUpdated = true;
			}
		}
	} while (!corner.Equals(room.corners[0]) && counter < 100);
}

void ARoomGeneration::FindFirstRoomAppearance(int rep, TArray<int>& level, FVector2D& point) {
	for (int y = 0; y < this->LevelHeight; y++) {
		for (int x = 0; x < this->LevelWidth; x++) {
			if (GetLevelElement(level, x, y) == rep) {
				point.X = x;
				point.Y = y;
				return;
			}
		}
	}
}
