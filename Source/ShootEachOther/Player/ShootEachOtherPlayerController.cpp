// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShootEachOtherPlayerController.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "GameplayAbility/SEO_AbilitySet.h"
#include "Character/SEOPawnData.h"

#include "SEO_PlayerState.h"
#include "SEO_PlayerComponent.h"
#include "SEO_GlobalFunctionLibrary.h"



USEOAbilitySystemComponent* AShootEachOtherPlayerController::GetSEOAbilitySystemComponent() const
{
	const ASEO_PlayerState* playerState = GetSEOPlayerState();
	return playerState ? playerState->GetSEOAbilitySystemComponent() : nullptr;
}

ASEO_PlayerState* AShootEachOtherPlayerController::GetSEOPlayerState() const
{
	return CastChecked<ASEO_PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void AShootEachOtherPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShootEachOtherPlayerController::Tick(float deltatime)
{
	Super::Tick(deltatime);
}

void AShootEachOtherPlayerController::OnPossess(APawn* PawnToProcess)
{
	Super::OnPossess(PawnToProcess);
	USEOAbilitySystemComponent* asc = GetSEOAbilitySystemComponent();
	if (asc) {
		asc->SetAvatarActor(PawnToProcess);

		if (asc->IsOwnerActorAuthoritative()) {
			if (const USEO_PlayerComponent* upc = PawnToProcess->GetComponentByClass<USEO_PlayerComponent>()) {
				if (!upc->PawnData) {
					USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "Invalid Pawn Data");
					return;
				}
				if (USEO_AbilitySet* Ability = upc->PawnData->AbilitySet) {
					Ability->GiveAbilityToASC(asc);
					USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "give ability to component success");
				}
		
			}
		}
	}
	
}

void AShootEachOtherPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (USEOAbilitySystemComponent* asc = GetSEOAbilitySystemComponent()) {
		asc->ProcessAllAbility(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
