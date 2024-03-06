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

	void InitializeWeaponInstance(const EWeaponType& type,const FWeaponData& Data);

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

	/*Grant the ability, create and attach the weapon blueprint to player*/
	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	AWeaponBase* InitializeForWeapon(USEOAbilitySystemComponent* asc, AShootEachOtherCharacter* avatarActor);

	UFUNCTION(BlueprintPure, Category = "WeaponInstance")
	EWeaponType GetWeaponType() const;

	//UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	void GiveAbilityToASC(USEOAbilitySystemComponent* asc);

	virtual bool IsSupportedForNetworking() const override { return true; }
	//virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	//UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	void ClearAbilityFromASC(USEOAbilitySystemComponent* asc);

	ETraceSourceType TraceType = ETraceSourceType::ShootFromCameraToward;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	/*Defaults Weapon Data*/
	UPROPERTY(Replicated)
	FWeaponData DefaultsData;

	EWeaponType WeaponType;

	/*Use for grant ability in order to explicily clear input*/
	int32 UniqueInputID;

protected:
	virtual UWorld* GetWorld() const override
	{
		if (const UObject* MyOuter = GetOuter())
		{
			return MyOuter->GetWorld();
		}
		return nullptr;
	}

	UFUNCTION(BlueprintPure, Category = "My Object")
	AActor* GetOwningActor() const
	{
		return GetTypedOuter<AActor>();
	}

	// Call "Remote" (aka, RPC) functions through the actors NetDriver
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override
	{
		check(!HasAnyFlags(RF_ClassDefaultObject));
		AActor* Owner = GetOwningActor();
		UNetDriver* NetDriver = Owner->GetNetDriver();
		if (NetDriver)
		{
			NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
			return true;
		}
		return false;
	}
};
