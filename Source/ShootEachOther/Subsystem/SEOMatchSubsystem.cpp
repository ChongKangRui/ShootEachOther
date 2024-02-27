// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SEOMatchSubsystem.h"

void USEOMatchSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	UE_LOG(LogTemp, Error, TEXT("Match system initialize finished!!!!!!!!!"));

	if (const UWorld* world = GetWorld()) {
		if (world->GetMapName().Contains("Match")) {

		}
	}
}
