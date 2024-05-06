// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootEachOtherCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/SEO_PlayerComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Player/ShootEachOtherPlayerController.h"
#include "Player/SEO_PlayerState.h"
#include "AI/AIBotController.h"
#include "GameplayTagCollection.h"
#include "SEO_GlobalFunctionLibrary.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AShootEachOtherCharacter::AShootEachOtherCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
}

AShootEachOtherPlayerController* AShootEachOtherCharacter::GetSEOPlayerController() const
{
	return GetSEOPlayerState()->IsABot() ? nullptr : 
		CastChecked<AShootEachOtherPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

ASEO_PlayerState* AShootEachOtherCharacter::GetSEOPlayerState() const
{
	return CastChecked<ASEO_PlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

USEOAbilitySystemComponent* AShootEachOtherCharacter::GetSEOAbilitySystemComponent() const
{
	return Cast<USEOAbilitySystemComponent>(GetAbilitySystemComponent());;
}

AAIBotController* AShootEachOtherCharacter::GetBotController() const
{
	return GetSEOPlayerState()->IsABot() ? CastChecked<AAIBotController>(Controller, ECastCheckedType::NullAllowed) : nullptr;
}

void AShootEachOtherCharacter::OnCharacterDeath_Implementation()
{
	if (GetPlayerState()->IsABot()) {
		if(HasAuthority())
			GetBotController()->StopMovement();
	}
	else {
		if (IsLocallyControlled()) {
			GetSEOAbilitySystemComponent()->AbilityInputTagPressed(GameplayTagsCollection::GameplayEvent_Death);
			//GetSEOAbilitySystemComponent()->AddLooseGameplayTag(GameplayTagsCollection::TAG_Gameplay_AbilityInputBlocked);
			GetSEOAbilitySystemComponent()->AddLooseGameplayTag(GameplayTagsCollection::Status_Death);
		}
	}
}

UAbilitySystemComponent* AShootEachOtherCharacter::GetAbilitySystemComponent() const
{
	const ASEO_PlayerState* ps = GetSEOPlayerState();
	if (ps) {
		return ps->GetAbilitySystemComponent();
	}
	USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "Invalid ability system component get from player state");
	return nullptr;
}

APlayerCameraManager* AShootEachOtherCharacter::GetFirstPersonCameraManager() const
{
	if (const APlayerController* apc = Cast<APlayerController>(GetController())) {
		return apc->PlayerCameraManager;
	}
	return nullptr;
}


