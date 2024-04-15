// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_GameplayAbility.h"
#include "Weapon/WeaponInstance.h"
#include "Player/SEO_PlayerComponent.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Character/WeaponInventoryComponent.h"

#include "GameplayTagCollection.h"

#include "GameMode/GameState/SEO_GameState.h"
#include "SEO_GlobalFunctionLibrary.h"

const UWeaponInstance* USEO_GameplayAbility::GetEquippedWeaponInstance() const
{
	if (const UWeaponInventoryComponent* wic = GetWeaponInventoryComponent()) {
		return wic->GetCurrentWeaponInstance();
	}
	return nullptr;
}

UWeaponInstance* USEO_GameplayAbility::GetEquippedWeaponInstance()
{
	if (UWeaponInventoryComponent* wic = GetWeaponInventoryComponent()) {
		return wic->FindWeaponBySlot(wic->ActivatingSlot);
	}
	return nullptr;
}

UWeaponInventoryComponent* USEO_GameplayAbility::GetWeaponInventoryComponent() const
{
	if (GetAvatarActorFromActorInfo()) {
		if (UWeaponInventoryComponent* wic = GetAvatarActorFromActorInfo()->FindComponentByClass<UWeaponInventoryComponent>()) {
			return wic;
		}
	}
	return nullptr;
}



const USEO_PlayerComponent* USEO_GameplayAbility::GetPlayerComponent() const
{
	if (GetAvatarActorFromActorInfo()) {
		if (const USEO_PlayerComponent* upc = GetAvatarActorFromActorInfo()->FindComponentByClass<USEO_PlayerComponent>()) {
			return upc;
		}
	}
	return nullptr;
}



AShootEachOtherCharacter* USEO_GameplayAbility::GetSEOCharacter() const
{
	return CastChecked<AShootEachOtherCharacter>(GetAvatarActorFromActorInfo(), ECastCheckedType::NullAllowed);
}

USEOAbilitySystemComponent* USEO_GameplayAbility::GetSEOAbilitySystemComponent() const
{
	
	return (CurrentActorInfo ? Cast<USEOAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);;
}

const ASEO_GameState* USEO_GameplayAbility::GetSEOGameState() const
{
	if (GetWorld()) {
		if (const AGameModeBase* gm = GetWorld()->GetAuthGameMode()) {
			if (const ASEO_GameState* gs = gm->GetGameState<ASEO_GameState>()) {
				return gs;
			}
		}
	}
	return nullptr;
}

void USEO_GameplayAbility::ApplyDamageToTarget_Implementation(const float Damage, AActor* HitActor)
{
	/*Apply damage gameplay effect to target*/

	bool HasFriendlyDamage = false;
	if (GetSEOGameState()) {
		HasFriendlyDamage = GetSEOGameState()->GetMatchSetting().HasFriendlyDamage;
	}
	USEO_GlobalFunctionLibrary::ApplyDamageToTarget(Damage, DamageGE, GetAvatarActorFromActorInfo(), HitActor, HasFriendlyDamage);

}