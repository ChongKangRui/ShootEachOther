// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WeaponInventoryComponent.h"
#include "Character/ShootEachOtherCharacter.h"

#include "Engine/ActorChannel.h"
#include "Engine/DataTable.h"
#include "Weapon/WeaponInstance.h"
#include "Weapon/WeaponBase.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "SEO_GlobalFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagCollection.h"


// Sets default values for this component's properties
UWeaponInventoryComponent::UWeaponInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	/*Initialize Weapon Slots*/
	if (AShootEachOtherCharacter* pawn = Cast<AShootEachOtherCharacter>(GetOwner())) {
		if (pawn->HasAuthority()) {
		
			WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Primary));
			WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Secondary));
			WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Melee));
			WeaponSlotData.Add(FWeaponSlot(EWeaponSlotType::Grenade));
			ActivatingSlot = EWeaponSlotType::None;
		}
	}
	
}

void UWeaponInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ActivatingSlot);
	DOREPLIFETIME(ThisClass, WeaponSlotData);
	DOREPLIFETIME(ThisClass, AttachedWeapon);
}

bool UWeaponInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool out = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (FWeaponSlot& data : WeaponSlotData) {
		Channel->ReplicateSubobject(data.WeaponInstance, *Bunch, *RepFlags);
	}
	return out;
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

const UWeaponInstance* UWeaponInventoryComponent::GetCurrentWeaponInstance() const
{
	return FindWeaponBySlot(ActivatingSlot);
}

void UWeaponInventoryComponent::SwapWeaponInstance(const EWeaponSlotType ReplacementWeaponSlot, UWeaponInstance* Replacement)
{
	if (Replacement) {

		bool ShouldSwapToCurrentWeapon = (ReplacementWeaponSlot == ActivatingSlot);

		for (FWeaponSlot& ws : WeaponSlotData) {
			if (ws.SlotType == ReplacementWeaponSlot) {
				if (ShouldSwapToCurrentWeapon) {
					UWeaponInstance* pwi = FindWeaponBySlot(ActivatingSlot);
					SwitchCurrentWeapon(Replacement, pwi, false);
				}
				ws.WeaponInstance = Replacement;
				break;
				
			}
		}
	}
}

void UWeaponInventoryComponent::SwitchCurrentWeapon_Implementation(UWeaponInstance* ReplacementInstance, UWeaponInstance* ToReplace, bool UseBlueprintBindFunction)
{
	if (AShootEachOtherCharacter* pawn = Cast<AShootEachOtherCharacter>(GetOwner())) {
		if (USEOAbilitySystemComponent* asc = pawn->GetSEOAbilitySystemComponent()) {

			if (!ReplacementInstance) {
				USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Invalid replacement weapon instance reference");
				return;
			}
			if (ToReplace) {
				ToReplace->ClearAbilityFromASC(asc);
				AttachedWeapon->Destroy();
			}
			if (UseBlueprintBindFunction) {
				OnWeaponChanged_Multicast(true, ReplacementInstance);
			}
			else {
				AttachedWeapon = ReplacementInstance->InitializeForWeapon(asc, pawn);
			}

			/*Make sure activating slot is alway the correct slot*/
			ActivatingSlot = ReplacementInstance->GetDefaultsWeaponData().EWeaponSlotType;
			USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Switch Weapon Success");
		}
	
	}


}

void UWeaponInventoryComponent::SetCurrentWeaponSlot_Implementation(const EWeaponSlotType WeaponSlot, bool UseBlueprintBindFunction)
{
	if (WeaponSlot == ActivatingSlot) {
		UE_LOG(LogTemp, Warning, TEXT("Same Slot, no trigger happen"));
		OnWeaponChanged_Multicast(false, nullptr);
		return;
	}
	UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot);

	if (!wi) {
		FString str = UEnum::GetValueAsString(WeaponSlot);
		USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "No weapon instance in slot");
		OnWeaponChanged_Multicast(false, nullptr);
		return;
	}

	SwitchCurrentWeapon(wi, FindWeaponBySlot(ActivatingSlot), UseBlueprintBindFunction);
}


void UWeaponInventoryComponent::ResetWeaponSlotToDefault()
{
	ClearAllWeaponSlotInstance();

	AddWeaponToSlot(EWeaponType::Secondary_Pistol);
	AddWeaponToSlot(EWeaponType::Melee_DefaultsKnife);
	
	SetCurrentWeaponSlot(EWeaponSlotType::Secondary);

}

void UWeaponInventoryComponent::AddWeaponToSlot_Implementation(const EWeaponType WeaponType, bool ReplaceWeapon)
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

	if (!ReplaceWeapon) {
		if (FindWeaponBySlot(data->EWeaponSlotType)) {
			UE_LOG(LogTemp, Error, TEXT("Weapon Slot aldy contain weapon data!!!"));
			return;
		}
	}
	//Create Weapon instance
	UWeaponInstance* WI = NewObject<UWeaponInstance>(GetOwner());
	WI->InitializeWeaponInstance(WeaponType, *data);

	SwapWeaponInstance(data->EWeaponSlotType, WI);
}

void UWeaponInventoryComponent::DropWeaponFromSlot_Implementation(const EWeaponSlotType WeaponSlotType)
{
}

void UWeaponInventoryComponent::RemoveWeaponFromSlot_Implementation(const EWeaponSlotType WeaponSlotType)
{
	for (FWeaponSlot& slot : WeaponSlotData) {
		if (slot.SlotType == WeaponSlotType) {
			slot.WeaponInstance = nullptr;
			
		}
	}
}

void UWeaponInventoryComponent::ClearAllWeaponSlotInstance_Implementation()
{
	for (FWeaponSlot& slot : WeaponSlotData) {
		slot.WeaponInstance = nullptr;
	}
}

void UWeaponInventoryComponent::OnWeaponChanged_Server_Implementation(bool SwitchWeaponSuccess, UWeaponInstance* ReplacementInstance)
{
	OnWeaponChanged_Multicast(SwitchWeaponSuccess, ReplacementInstance);
}

void UWeaponInventoryComponent::OnWeaponChanged_Multicast_Implementation(bool SwitchWeaponSuccess, UWeaponInstance* ReplacementInstance)
{
	OnWeaponEquip.Broadcast(SwitchWeaponSuccess, ReplacementInstance);
}



