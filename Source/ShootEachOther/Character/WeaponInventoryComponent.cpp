// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WeaponInventoryComponent.h"
#include "Character/ShootEachOtherCharacter.h"

#include "Engine/DataTable.h"
#include "Weapon/WeaponInstance.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Player/ShootEachOtherPlayerController.h"
#include "SEO_GlobalFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponInventoryComponent::UWeaponInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// ...
	WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Primary));
	WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Secondary));
	WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Melee));
	WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Grenade));

	ActivatingSlot = EWeaponSlotType::None;

	
}


// Called when the game starts
void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetWeaponSlotToDefault();

}


// Called every frame
void UWeaponInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ThisClass, WeaponSlotData);
	DOREPLIFETIME(ThisClass, ActivatingSlot);
	DOREPLIFETIME(ThisClass, AttachedWeapon);
}

UWeaponInstance* UWeaponInventoryComponent::FindWeaponBySlot(const EWeaponSlotType type)
{
	for (FWeaponSlot& wi : WeaponSlotData) {
		if (wi.SlotType == type)
			return wi.WeaponInstance;
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to find weapon with not exist enum type. ");
	return nullptr;
}

const UWeaponInstance* UWeaponInventoryComponent::FindWeaponBySlot(EWeaponSlotType type) const
{
	for (const FWeaponSlot& wi : WeaponSlotData) {
		if (wi.SlotType == type)
			return wi.WeaponInstance;
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to find weapon with not exist enum type. ");
	return nullptr;
}



AWeaponBase* UWeaponInventoryComponent::GetCurrentAttachedWeapon() const
{
	return AttachedWeapon;
}

FWeaponData UWeaponInventoryComponent::GetCurrentWeaponData() const
{
	return FWeaponData();
}

const UWeaponInstance* UWeaponInventoryComponent::GetCurrentWeaponInstance() const
{
	return FindWeaponBySlot(ActivatingSlot);
}


FWeaponData UWeaponInventoryComponent::GetWeaponDataFromSlot(const EWeaponSlotType WeaponSlot) const
{
	
	for (const FWeaponSlot& weaponData : WeaponSlotData) {
		if (weaponData.SlotType == WeaponSlot) {
			if (!weaponData.WeaponInstance) {
				USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to find weapon data with Invalid Weapon Instance");
				return FWeaponData();
			}
			return weaponData.WeaponInstance->GetDefaultsWeaponData();
		}
	}

	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to find weapon with unknown enum type. ");
	return FWeaponData();
}
void UWeaponInventoryComponent::ResetWeaponSlotToDefault()
{
	for (auto& weaponSlot : WeaponSlotData) {
		weaponSlot.WeaponInstance = nullptr;
	}

	AddWeaponToSlot(EWeaponType::Pistol);
	SetCurrentWeaponSlot(EWeaponSlotType::Secondary);

}


void UWeaponInventoryComponent::SetCurrentWeaponSlot_Implementation(const EWeaponSlotType WeaponSlot)
{
	if (WeaponSlot == ActivatingSlot) {
		UE_LOG(LogTemp, Warning, TEXT("Same Slot, no trigger happen"));
		return;
	}
	UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot);

	if (!wi) {
		FString str = UEnum::GetValueAsString(WeaponSlot);
		UE_LOG(LogTemp, Warning, TEXT("No weapon instance in %s slot"), *str);
		return;
	}

	if (AShootEachOtherCharacter* pawn = Cast<AShootEachOtherCharacter>(GetOwner())) {
		if (const AShootEachOtherPlayerController* controller = Cast<AShootEachOtherPlayerController>(pawn->Controller)){
			if (USEOAbilitySystemComponent* asc = controller->GetSEOAbilitySystemComponent()) {
				UWeaponInstance* pwi = FindWeaponBySlot(ActivatingSlot);
				if(ActivatingSlot != EWeaponSlotType::None && pwi)
					pwi->ClearAbilityFromASC(asc);
				
				//WeaponSlotData[WeaponSlot]->GiveAbilityToASC(asc);
				AttachedWeapon = wi->InitializeForWeapon(asc, pawn);
				
				/*Spawn gun and attach to character*/
				USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Ini weapon finish");
				
				ActivatingSlot = WeaponSlot;
			}
			else {
				USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Invalid ASC");
			}

		}
		else
			USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Invalid controller");

	}
	else
		USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "invalid pawn");
}

void UWeaponInventoryComponent::ClearWeaponSlotData(const EWeaponSlotType WeaponSlot)
{
	for (FWeaponSlot& wi : WeaponSlotData) {
		if (wi.SlotType == WeaponSlot)
			wi.WeaponInstance = nullptr;
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to clear weapon with None enum type. ");
}

void UWeaponInventoryComponent::AddWeaponToSlot(const EWeaponType WeaponType, bool ReplaceWeapon)
{

	if (!WeaponDataTable) {
		UE_LOG(LogTemp, Error, TEXT("Invalid DataTable"));
		return;
	}

	FString weaponTypeString = UEnum::GetValueAsString(WeaponType);
	weaponTypeString.RemoveFromStart("EWeaponType::");
	const FWeaponData* data = WeaponDataTable->FindRow<FWeaponData>(FName(weaponTypeString), TEXT("Searching weapon data from data table"));

	//Check if data invalid
	if (!data) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Enum: %s"), *weaponTypeString);
		return;
	}

	//Check if datatable enum setup incorrect
	if (data->EWeaponSlotType == EWeaponSlotType::None) {
		UE_LOG(LogTemp, Error, TEXT("Weapon Slot Type is None!!!!"));
		return;
	}

	//May replace to drop weapon function later


	if (!ReplaceWeapon) {
		if (FindWeaponBySlot(data->EWeaponSlotType)) {
			UE_LOG(LogTemp, Error, TEXT("Weapon Slot aldy contain weapon data!!!"));
			return;
		}
	}

	//Create Weapon instance
	UWeaponInstance* WI = NewObject<UWeaponInstance>();
	WI->InitializeWeaponInstance(*data);

	for (FWeaponSlot& ws : WeaponSlotData) {
		if (ws.SlotType == data->EWeaponSlotType)
			ws.WeaponInstance = WI;
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Warning, "Finished Weapon ADDed");
}



/// Weapon Stat Related
int32 UWeaponInventoryComponent::GetWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag) const
{
	if (const UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot)){
		return wi->GetStatCount(StatTag);
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Weapon Slot may not contain data to Get Stat!!!");
	return -1;
}

int32 UWeaponInventoryComponent::GetCurrentWeaponStatCount(const FGameplayTag StatTag) const
{
	return GetWeaponStatCount(ActivatingSlot, StatTag);
}

void UWeaponInventoryComponent::AddWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag, const int32 value)
{
	if (UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot)) {
		wi->AddStatCount(StatTag, value);
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Weapon Slot may not contain data to Add Stat!!!");
}

void UWeaponInventoryComponent::AddCurrentWeaponStatCount(const FGameplayTag StatTag, const int32 value)
{
	AddWeaponStatCount(ActivatingSlot, StatTag, value);
}

void UWeaponInventoryComponent::RemoveWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag, const int32 value)
{
	if (UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot)) {
		wi->RemoveStatCount(StatTag, value);
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Weapon Slot may not contain data to Remove Stat!!!");
}

void UWeaponInventoryComponent::RemoveCurrentWeaponStatCount(const FGameplayTag StatTag, const int32 value)
{
	RemoveWeaponStatCount(ActivatingSlot, StatTag, value);
}

/// Weapon Stat End Related




