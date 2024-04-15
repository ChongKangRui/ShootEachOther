// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBotController.h"

#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "GameplayAbility/SEO_AbilitySet.h"
#include "Character/SEOPawnData.h"
#include "Character/ShootEachOtherCharacter.h"

#include "Player/SEO_PlayerState.h"
#include "Player/SEO_PlayerComponent.h"

#include "SEO_GlobalFunctionLibrary.h"

AAIBotController::AAIBotController()
{
	bWantsPlayerState = true;
}

void AAIBotController::OnPossess(APawn* PawnToProcess)
{
	Super::OnPossess(PawnToProcess);

	AShootEachOtherCharacter* character = Cast<AShootEachOtherCharacter>(PawnToProcess);

	USEOAbilitySystemComponent* asc = character->GetSEOAbilitySystemComponent();
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