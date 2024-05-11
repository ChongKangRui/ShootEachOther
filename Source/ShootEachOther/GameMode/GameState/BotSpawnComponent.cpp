// Fill out your copyright notice in the Description page of Project Settings.


#include "BotSpawnComponent.h"
#include "GameMode/ShootEachOtherGameMode.h"

#include "AIController.h"
#include "SEO_GameState.h"

#include "Player/SEO_PlayerState.h"


UBotSpawnComponent::UBotSpawnComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicated(true);
}

void UBotSpawnComponent::BeginPlay()
{
	owner = GetGameState<ASEO_GameState>();
}

void UBotSpawnComponent::AddOneBot(const int32 TeamID, const int Index)
{
	if (!owner) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Game State Ref from BotSpawnComponent"));
		return;
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAIController* NewController = GetWorld()->SpawnActor<AAIController>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	if (NewController) {
		if (ASEO_PlayerState* ps = Cast<ASEO_PlayerState>(NewController->PlayerState)) {
			
			ps->SetGenericTeamId(FGenericTeamId(TeamID));
			owner->AddPlayerToTeam(ps, TeamID, Index);

			UE_LOG(LogTemp, Error, TEXT("Add Bot Success"));
		}
	}
	
}

void UBotSpawnComponent::RemoveBot(const int32 TeamID, const int Index)
{
	owner->GetTeamInfo(TeamID).Players[Index]->GetOwningController()->Destroy();
}


