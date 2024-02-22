// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/SEO_GameplayAbility.h"
#include "Weapon/WeaponData.h"
#include "SGA_Aim.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USGA_Aim : public USEO_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetTraceState(const ETraceSourceType TraceType);
};
