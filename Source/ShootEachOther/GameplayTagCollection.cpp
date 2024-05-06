// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagCollection.h"
#include "GameplayTagsManager.h"

namespace GameplayTagsCollection {

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Move, "Input.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Look, "Input.Look", "Mouse Look.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Shoot, "Input.Shoot", "Shoot.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Shoot_Auto, "Input.Shoot.Auto", "Auto Shoot.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Aim, "Input.Aim", "Aim.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Slot, "Input.Slot", "Weapon Slot");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Define if character dead");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Reset, "GameplayEvent.Reset", "Event that fires once a player reset is executed.");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponDamage, "Weapon.Damage", "Use this value to apply the damage to target health, this should used for set magnitube by caller");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponCurrentAmmo, "Weapon.Ammo.Current", "Use to represent the current ammo.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponCurrentSpareAmmo, "Weapon.Ammo.CurrentSpare", "Use to represent the current spare ammo.");

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}