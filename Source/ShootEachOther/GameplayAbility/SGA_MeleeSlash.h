// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/SEO_GameplayAbility.h"
#include "SGA_MeleeSlash.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USGA_MeleeSlash : public USEO_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageGE;

	UFUNCTION(BlueprintCallable)
	void MeleeTrace();

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float DebugDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float HitPointThickness = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float LineThickness = 1.0f;
	
private:
	UFUNCTION(Server, Reliable)
	void ApplyDamageToTarget(const float Damage, AActor* HitActor);
};
