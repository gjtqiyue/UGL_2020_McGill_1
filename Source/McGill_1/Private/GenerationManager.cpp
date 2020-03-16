// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerationManager.h"
UGenerationNode* CreateRoomGeneration(AGenerationManager& manager);
// Sets default values
AGenerationManager::AGenerationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGenerationManager::BeginPlay()
{
	Super::BeginPlay();
//	ParseGenerationGrammar(FString("/test.txt"));
	auto room = CreateRoomGeneration(*this);
	this->Generate(*room);
}

// Called every frame
void AGenerationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UGenerationNode* CreateRoomGeneration(AGenerationManager& manager) {
	auto room = UGenerationNode::Create("Room", nullptr);
	auto item = UGenerationNode::Create("Item", &manager.item);
	auto space = UGenerationNode::Create("Space",&manager.floor);
	GenerationProperty x = { 0, 10 };
	GenerationProperty y = { 0, 10 };
	GenerationProperty z = { 0, 10 };
	GenerationProperty h = { 0, 10 };

	item->properties = TArray<GenerationProperty>();
	item->properties.Add(x);
	item->properties.Add(y);
	item->properties.Add(z);
	room->children = TArray <TArray<UGenerationNode*>>();
	auto li = TArray<UGenerationNode*>();
	li.Add(item);
	li.Add(space);
	room->children.Add(li);
	return room;
}

void AGenerationManager::Generate(UGenerationNode& node) {
	if (node.ActorClass != nullptr) {
		UWorld* world = this->GetWorld();
		world->SpawnActor<AActor>(*node.ActorClass, FVector(0, 0, 0), FRotator(0, 0, 0));
	}
	else {

		for (int i = 0; i < node.children.Num(); i++) {
			for (int j = 0; j < node.children[i].Num(); j++) {
				Generate(*node.children[i][j]);
			}
		}
	}
}