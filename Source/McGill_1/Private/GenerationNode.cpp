// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerationNode.h"

void ParseLine(FString& line, TMap<FString, UGenerationNode>& context);
UGenerationNode::UGenerationNode() {

}

UGenerationNode* UGenerationNode::Create(FString name, TSubclassOf<AActor>* actorClass) {
	UGenerationNode* node = NewObject<UGenerationNode>();
	node->Name = name;
	node->ActorClass = actorClass;
	return node;
}

// in progress
void ParseGenerationGrammar(FString fileName) {
	FString content = FString();
	FString path = FPaths::ProjectContentDir();
	path += fileName;
	FFileHelper::LoadFileToString(content, *path);
	TArray<FString> lines = TArray<FString>();
	content.ParseIntoArray(lines, TEXT("\n"));
	TMap<FString, UGenerationNode> context;


	for (int i = 0; i < lines.Num(); i++) {
		if (lines[i].IsEmpty()) {
			continue;
		}
		ParseLine(lines[i], context);

	}


	//UE_LOG(LogTemp, Warning, TEXT("That %s"), *content);
}

// in progress
void ParseLine(FString& line, TMap<FString, UGenerationNode>& context) {
	auto splited = line.GetCharArray();

	// find node names
	TArray<TCHAR> stack;
	int nodeCount = 0;
	for (int i = 0; i < splited.Num(); i++) {
		if (splited[i] == '>') {
			FString info = FString();
			nodeCount += 1;
			while (stack[stack.Num() - 1] != '<') {
				int endIndex = stack.Num() - 1;
				info.AppendChar(stack[endIndex]);
				stack.Remove(endIndex);
			}
			info = info.Reverse();
		}



		
	}

}