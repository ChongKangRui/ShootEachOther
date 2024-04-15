// Fill out your copyright notice in the Description page of Project Settings.


#include "BotSpawnComponent.h"
#include "GameMode/ShootEachOtherGameMode.h"

#include "AIController.h"
#include "SEO_GameState.h"

#include "Player/SEO_PlayerState.h"

//void ULyraBotCreationComponent::SpawnOneBot()
//{
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnInfo.OverrideLevel = GetComponentLevel();
//	SpawnInfo.ObjectFlags |= RF_Transient;
//	AAIController* NewController = GetWorld()->SpawnActor<AAIController>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
//
//	if (NewController != nullptr)
//	{
//		ALyraGameMode* GameMode = GetGameMode<ALyraGameMode>();
//		check(GameMode);
//
//		if (NewController->PlayerState != nullptr)
//		{
//			NewController->PlayerState->SetPlayerName(CreateBotName(NewController->PlayerState->GetPlayerId()));
//		}
//
//		GameMode->GenericPlayerInitialization(NewController);
//		GameMode->RestartPlayer(NewController);
//
//		if (NewController->GetPawn() != nullptr)
//		{
//			if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
//			{
//				PawnExtComponent->CheckDefaultInitialization();
//			}
//		}
//
//		SpawnedBotList.Add(NewController);
//	}
//}
//
//void ULyraBotCreationComponent::RemoveOneBot()
//{
//	if (SpawnedBotList.Num() > 0)
//	{
//		// Right now this removes a random bot as they're all the same; could prefer to remove one
//		// that's high skill or low skill or etc... depending on why you are removing one
//		const int32 BotToRemoveIndex = FMath::RandRange(0, SpawnedBotList.Num() - 1);
//
//		AAIController* BotToRemove = SpawnedBotList[BotToRemoveIndex];
//		SpawnedBotList.RemoveAtSwap(BotToRemoveIndex);
//
//		if (BotToRemove)
//		{
//			// If we can find a health component, self-destruct it, otherwise just destroy the actor
//			if (APawn* ControlledPawn = BotToRemove->GetPawn())
//			{
//				if (ULyraHealthComponent* HealthComponent = ULyraHealthComponent::FindHealthComponent(ControlledPawn))
//				{
//					// Note, right now this doesn't work quite as desired: as soon as the player state goes away when
//					// the controller is destroyed, the abilities like the death animation will be interrupted immediately
//					HealthComponent->DamageSelfDestruct();
//				}
//				else
//				{
//					ControlledPawn->Destroy();
//				}
//			}
//
//			// Destroy the controller (will cause it to Logout, etc...)
//			BotToRemove->Destroy();
//		}
//	}
//}

void UBotSpawnComponent::AddOneBot(const int32 TeamID, const int Index)
{
	if (!owner) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Game State Ref from BotSpawnComponent"));
		return;
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

//	SpawnInfo.ObjectFlags |= RF_Transient;

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


UBotSpawnComponent::UBotSpawnComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicated(true);
}

void UBotSpawnComponent::BeginPlay()
{
	owner = GetGameState<ASEO_GameState>();
}
