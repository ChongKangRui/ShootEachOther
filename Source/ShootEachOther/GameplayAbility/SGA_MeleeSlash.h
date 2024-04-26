// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/SEO_GameplayAbility.h"
#include "Weapon/WeaponData.h"
#include "SGA_MeleeSlash.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USGA_MeleeSlash : public USEO_GameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MeleeTrace();

	void HandleDamageToEnemy(TArray<FHitResult>& HitResults, const FWeaponData& Data);
public:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float DebugDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float HitPointThickness = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float LineThickness = 1.0f;
};
