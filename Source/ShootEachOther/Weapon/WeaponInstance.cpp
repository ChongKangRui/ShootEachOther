// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameplayAbility/SEO_AbilitySet.h"
#include "GameplayAbility/SEO_GameplayAbility.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Weapon/WeaponBase.h"
#include "Misc/Guid.h"
#include "GameplayTagContainer.h"
#include "Character/ShootEachOtherCharacter.h"



UWeaponInstance::UWeaponInstance()
{
	
}

void UWeaponInstance::InitializeWeaponInstance(const EWeaponType& type, const FWeaponData& Data)
{
	
	DefaultsData = Data;
	WeaponType = type;
	{
		int32 id = -1;
		switch (DefaultsData.EWeaponSlotType) {
		case EWeaponSlotType::Primary:
			id = 0;
			break;
		case EWeaponSlotType::Secondary:
			id = 1;
			break;
		case EWeaponSlotType::Melee:
			id = 2;
			break;
		case EWeaponSlotType::Grenade:
			id = 3;
			break;

		}
		
		UniqueInputID = id;
	}

	//Grant Stat Value
	for (const FStackValue& pair : DefaultsData.StackValue) {
		AddStatCount(pair.Tag, pair.Value);
	}
	
}

FWeaponData UWeaponInstance::GetDefaultsWeaponData() const
{
	return DefaultsData;
}

int32 UWeaponInstance::GetStatCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UWeaponInstance::IsContainTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UWeaponInstance::AddStatCount(FGameplayTag Tag, int32 Amount)
{
	StatTags.AddStack(Tag, Amount);
}

void UWeaponInstance::RemoveStatCount(FGameplayTag Tag, int32 Amount)
{
	StatTags.RemoveStack(Tag, Amount);
}

void UWeaponInstance::GiveAbilityToASC(USEOAbilitySystemComponent* asc)
{
	if (asc) {
		for (const FAbilitySet_GameplayAbility& GA : DefaultsData.WeaponAbilitySet) {
			if (!GA.Ability || !GA.InputTag.IsValid()) {
				UE_LOG(LogTemp, Error, TEXT("Invalid Input Tag or Ability"));
				continue;
			}

			if (!asc->HasMatchingGameplayTag(GA.InputTag)) {
				USEO_GameplayAbility* ability = GA.Ability->GetDefaultObject<USEO_GameplayAbility>();
				FGameplayAbilitySpec specToGrant(ability, GA.AbilityLevel);
				specToGrant.DynamicAbilityTags.AddTag(GA.InputTag);
				specToGrant.InputID = UniqueInputID;
				asc->GiveAbility(specToGrant);

				UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
				FString str = EnumPtr->GetNameStringByValue((int64)WeaponType);
				UE_LOG(LogTemp, Error, TEXT("giving ability at weapon: %s"), *str);
			}

		}
		UE_LOG(LogTemp, Error, TEXT("give ability success"));
	}
}

void UWeaponInstance::ClearAbilityFromASC(USEOAbilitySystemComponent* asc)
{

	asc->ClearAllAbilitiesWithInputID(UniqueInputID);
	UE_LOG(LogTemp, Error, TEXT("clear Unique input id = %i"), UniqueInputID);
	//
	
}

AWeaponBase* UWeaponInstance::InitializeForWeapon(USEOAbilitySystemComponent* asc, AShootEachOtherCharacter* avatarActor)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(avatarActor, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr) {
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Instigator = avatarActor;
		ActorSpawnParams.Owner = avatarActor;
		AWeaponBase* weapon = World->SpawnActor<AWeaponBase>(DefaultsData.WeaponClass, ActorSpawnParams);

		if (!weapon) {
			UE_LOG(LogTemp, Error, TEXT("Invalid Weapon"));
			return nullptr;
		}
		weapon->AttachToComponent(avatarActor->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, DefaultsData.AttachName);

		GiveAbilityToASC(asc);
		
		UE_LOG(LogTemp, Warning, TEXT("Set current weapon success"));

		return weapon;
	}
	return nullptr;
}

EWeaponType UWeaponInstance::GetWeaponType() const
{
	return WeaponType;
}

void UWeaponInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, DefaultsData);
	DOREPLIFETIME(ThisClass, WeaponType);
}





