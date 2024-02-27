// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponData.h"
#include "WeaponInventoryComponent.generated.h"

class AWeaponBase;
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
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquip, bool, EquipSuccess, UWeaponInstance*, ReplacementWeaponInstance);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTEACHOTHER_API UWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponInventoryComponent();

	/*Reset weapon slot to default setup
	Defaults setup will be pistol and melee only
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void ResetWeaponSlotToDefault();

	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void AddWeaponToSlot(const EWeaponType WeaponType, bool ReplaceWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void ClearWeaponSlotData(const EWeaponSlotType WeaponSlot);

	/*Return the weapon instance from current slot */
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	const UWeaponInstance* GetCurrentWeaponInstance() const;

	/*Return the weapon blueprint from current slot */
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	AWeaponBase* GetCurrentAttachedWeapon() const;

	/*Return the current weapon data that player holding */
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	FWeaponData GetCurrentWeaponData() const;

	/*Return the weapon data from specific slot */
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	FWeaponData GetWeaponDataFromSlot(const EWeaponSlotType WeaponSlot) const;

	/*Do all the work that related to weapon and it's data, spawn the represent blueprint to the player mesh, assign data to the weapon*/
	UFUNCTION(BlueprintCallable,Server, Reliable, Category = "Weapon Slot Component")
	void SetCurrentWeaponSlot(const EWeaponSlotType WeaponSlot, bool UseBlueprintBindFunction = false);

	/*Use this to get weapon ammo size/count etc, -1 mean it is invalid*/
	UFUNCTION(BlueprintPure, Category = "Weapon Slot Component")
	int32 GetWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag) const;

	/*Use this to get weapon ammo size/count etc, -1 mean it is invalid*/
	UFUNCTION(BlueprintPure, Category = "Weapon Slot Component")
	int32 GetCurrentWeaponStatCount(const FGameplayTag StatTag) const;

	/*Use this to add specific stat count*/
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void AddWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag, const int32 value);

	/*Use this to add current stat count*/
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void AddCurrentWeaponStatCount(const FGameplayTag StatTag, const int32 value);

	/*Use this to deduct specific stat count*/
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void RemoveWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag, const int32 value);

	/*Use this to deduct current stat count*/
	UFUNCTION(BlueprintCallable, Category = "Weapon Slot Component")
	void RemoveCurrentWeaponStatCount(const FGameplayTag StatTag, const int32 value);

	UFUNCTION(BlueprintPure, Category = "Weapon Slot Component")
	const UWeaponInstance* FindWeaponBySlot(EWeaponSlotType type) const;

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

	UPROPERTY(EditDefaultsOnly)
	UDataTable* WeaponDataTable;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TObjectPtr<AWeaponBase> AttachedWeapon;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*Responsible to swap the weapon instance reference when buying weapon or replace weapon instance*/
	void SwapWeaponInstance(const EWeaponSlotType ReplacementWeaponSlot, UWeaponInstance* Replacement);
	/*Helper function to change current weapon*/
	void SwitchCurrentWeapon(UWeaponInstance* ReplacementInstance,UWeaponInstance* ToReplace, bool UseBlueprintBindFunction);
	
	UWeaponInstance* FindWeaponBySlot(const EWeaponSlotType type);

	friend class USEO_GameplayAbility;
};
