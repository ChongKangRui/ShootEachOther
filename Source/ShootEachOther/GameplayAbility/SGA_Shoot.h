// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/SEO_GameplayAbility.h"
#include "Weapon/WeaponData.h"
#include "SGA_Shoot.generated.h"

/**
 * 
 */




UCLASS()
class SHOOTEACHOTHER_API USGA_Shoot : public USEO_GameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float DebugDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float HitPointThickness = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float LineThickness = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageGE;


public:

	UFUNCTION(BlueprintCallable)
	void StartWeaponTrace();

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, TArray<FHitResult>& HitResult, float SweepRadius, float DebugTime) const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponFired(const TArray<FHitResult>& HitResult);

	UFUNCTION(Server, Reliable)
	void OnWeaponFired_Server(const TArray<FHitResult>& HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void OnWeaponFired_MultiCast(const TArray<FHitResult>& HitResult);

	UFUNCTION(Server, Reliable)
	void ApplyDamageToTarget(const float Damage, AActor* HitActor);


	FVector GetTraceStart(const ETraceSourceType& type) const;
	FVector GetTraceDirection(const ETraceSourceType& type, const float& TraceDistance) const;
	


};
