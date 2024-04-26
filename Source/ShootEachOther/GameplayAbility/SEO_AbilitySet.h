// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "SEO_AbilitySet.generated.h"

/**
 * 
 */

class USEO_GameplayAbility;
class USEOAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USEO_GameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class SHOOTEACHOTHER_API USEO_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USEO_AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void GiveAbilityToASC(USEOAbilitySystemComponent* asc);

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	
};
