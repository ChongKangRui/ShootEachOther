// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_GameplayAbility.h"
#include "Weapon/WeaponInstance.h"
#include "Player/SEO_PlayerComponent.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Character/WeaponInventoryComponent.h"

#include "GameplayTagCollection.h"

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
