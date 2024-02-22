// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/SEO_AbilitySet.h"
#include "SEOAbilitySystemComponent.h"
#include "SEO_GameplayAbility.h"


USEO_AbilitySet::USEO_AbilitySet(const FObjectInitializer& ObjectInitializer)
{
	
}

void USEO_AbilitySet::GiveAbilityToASC(USEOAbilitySystemComponent* asc)
{
	for (FAbilitySet_GameplayAbility GA : GrantedGameplayAbilities) {
		if (!GA.Ability || !GA.InputTag.IsValid()) {
			UE_LOG(LogTemp, Error, TEXT("Invalid Input Tag or Ability"));
			continue;
		}

		USEO_GameplayAbility* ability = GA.Ability->GetDefaultObject<USEO_GameplayAbility>();
		FGameplayAbilitySpec specToGrant(ability, GA.AbilityLevel);
		specToGrant.DynamicAbilityTags.AddTag(GA.InputTag);
		asc->GiveAbility(specToGrant);

	}
}
