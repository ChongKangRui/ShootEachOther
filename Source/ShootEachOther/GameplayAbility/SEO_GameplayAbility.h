// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SEO_GameplayAbility.generated.h"

/**
 * 
 */
class UWeaponInstance;
class UWeaponInventoryComponent;
class USEO_PlayerComponent;
class AShootEachOtherCharacter;
class USEOAbilitySystemComponent;

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8 {
	WhenTriggered,
	WhenTriggerActivating,
	WhenSpawn
};


UCLASS()
class SHOOTEACHOTHER_API USEO_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	
	UFUNCTION(BlueprintPure, Category = "SEO|Ability")
	const UWeaponInstance* GetEquippedWeaponInstance() const;

	UFUNCTION(BlueprintPure, Category = "SEO|Ability")
	const USEO_PlayerComponent* GetPlayerComponent() const;

	UFUNCTION(BlueprintPure, Category = "SEO|Ability")
	UWeaponInventoryComponent* GetWeaponInventoryComponent() const;

	UFUNCTION(BlueprintPure, Category = "SEO|Ability")
	AShootEachOtherCharacter* GetSEOCharacter() const;

	UFUNCTION(BlueprintPure, Category = "SEO|Ability")
	USEOAbilitySystemComponent* GetSEOAbilitySystemComponent() const;

	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; };



protected:
	UWeaponInstance* GetEquippedWeaponInstance();
	


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EAbilityActivationPolicy ActivationPolicy;


};
