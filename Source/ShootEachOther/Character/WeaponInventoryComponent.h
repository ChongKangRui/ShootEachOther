// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponBase.h"
#include "WeaponInventoryComponent.generated.h"


class UDataTable;
class UWeaponInstance;


USTRUCT(BlueprintType)
struct SHOOTEACHOTHER_API FWeaponSlot {

	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	EWeaponSlotType SlotType;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWeaponInstance> WeaponInstance;

	FWeaponSlot() : SlotType(EWeaponSlotType::None) {};
	FWeaponSlot(EWeaponSlotType slot) : SlotType(slot) {}
	FWeaponSlot(EWeaponSlotType slot, UWeaponInstance* WI) : SlotType(slot), WeaponInstance(WI){}
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquip, bool, EquipSuccess, UWeaponInstance*, ReplacementWeaponInstance);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTEACHOTHER_API UWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void ResetWeaponSlotToDefault();

	/*Return the weapon instance from current slot */
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	const UWeaponInstance* GetCurrentWeaponInstance() const;

	/*Return the attached weapon blueprint from current slot */
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	AWeaponBase* GetCurrentAttachedWeapon() const;

	UFUNCTION(BlueprintPure, Category = "Weapon Slot Component")
	const UWeaponInstance* FindWeaponBySlot(EWeaponSlotType type) const;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Weapon Slot Component")
	void AddWeaponToSlot(const EWeaponType WeaponType, bool ReplaceWeapon = false);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Weapon Slot Component")
	void DropWeaponFromSlot(const EWeaponSlotType WeaponSlotType);

	/*This is difference from drop weapon since this should completely free the weapon reference without keeping it alive*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Weapon Slot Component")
	void RemoveWeaponFromSlot(const EWeaponSlotType WeaponSlotType);

	/*Do all the work that related to weapon and it's data, spawn the represent blueprint to the player mesh, assign data to the weapon*/
	UFUNCTION(BlueprintCallable,Server, Reliable, Category = "Weapon Slot Component")
	void SetCurrentWeaponSlot(const EWeaponSlotType WeaponSlot, bool UseBlueprintBindFunction = false);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponEquip OnWeaponEquip;

protected:
	/*
	slot 1: Main weapon
	slot 2: Secondary weapon
	slot 3: Melee
	slot 4: Grenade
	*/

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<FWeaponSlot> WeaponSlotData;

	UPROPERTY(BlueprintReadOnly, Replicated)
	EWeaponSlotType ActivatingSlot;
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	TObjectPtr<AWeaponBase> AttachedWeapon;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* WeaponDataTable;

protected:
	/*Helper function to change current weapon*/
	UFUNCTION(Server, Reliable)
	void SwitchCurrentWeapon(UWeaponInstance* ReplacementInstance, UWeaponInstance* ToReplace, bool UseBlueprintBindFunction);

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*Responsible to swap the weapon instance reference when buying weapon or replace weapon instance*/
	void SwapWeaponInstance(const EWeaponSlotType ReplacementWeaponSlot, UWeaponInstance* Replacement);
	UWeaponInstance* FindWeaponBySlot(const EWeaponSlotType type);

private:
	UFUNCTION(Server, Reliable)
	void OnWeaponChanged_Server(bool SwitchWeaponSuccess, UWeaponInstance* ReplacementInstance);

	UFUNCTION(NetMulticast, Reliable)
	void OnWeaponChanged_Multicast(bool SwitchWeaponSuccess, UWeaponInstance* ReplacementInstance);

	/*Set all weapon slot instance to nullptr*/
	UFUNCTION(Server, Reliable)
	void ClearAllWeaponSlotInstance();

	/*Use this to replicate weapon instance since weapon instance is an UObject*/
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;


	friend class USEO_GameplayAbility;

};
