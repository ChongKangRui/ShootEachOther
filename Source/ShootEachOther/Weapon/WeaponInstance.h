// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagStack.h"
#include "WeaponData.h"

#include "WeaponInstance.generated.h"

/**
 * 
 */
class AShootEachOtherCharacter;
class USEOAbilitySystemComponent;


UCLASS(BlueprintType, Blueprintable)
class SHOOTEACHOTHER_API UWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	UWeaponInstance();

	void InitializeWeaponInstance(const FWeaponData& Data);

	/*Return defaults weapon data*/
	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	FWeaponData GetDefaultsWeaponData() const;

	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	int32 GetStatCount(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	bool IsContainTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	void AddStatCount(FGameplayTag Tag, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	void RemoveStatCount(FGameplayTag Tag, int32 Amount);

	//UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	void GiveAbilityToASC(USEOAbilitySystemComponent* asc);

	//UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	void ClearAbilityFromASC(USEOAbilitySystemComponent* asc);

	AWeaponBase* InitializeForWeapon(USEOAbilitySystemComponent* asc, AShootEachOtherCharacter* avatarActor);

	ETraceSourceType TraceType = ETraceSourceType::ShootFromCameraToward;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	/*Defaults Weapon Data*/
	UPROPERTY(Replicated)
	FWeaponData DefaultsData;

	/*Use for grant ability in order to explicily clear input*/
	int32 UniqueInputID;
};
