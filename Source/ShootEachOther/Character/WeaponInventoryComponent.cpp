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
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
	SetIsReplicatedByDefault(true);
	// ...
	


}


// Called when the game starts
void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
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


// Called every frame
void UWeaponInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ThisClass, ActivatingSlot);
	DOREPLIFETIME(ThisClass, WeaponSlotData);
	DOREPLIFETIME(ThisClass, AttachedWeapon);
	
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
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "SwapWeapon UnSuccess");
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
		}
		else
			USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Invalid asc");
	}


}
#pragma region Getter Function

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

void UWeaponInventoryComponent::OnRep_SlotData()
{
	//UE_LOG(LogTemp, Error, TEXT("Replicccccccccccccc"));
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



#pragma endregion

#pragma region Setter Function

void UWeaponInventoryComponent::ResetWeaponSlotToDefault()
{
	
	//WeaponSlotData.Empty();
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
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Warning, "Finished Weapon ADDed");
}

void UWeaponInventoryComponent::DropWeaponFromSlot_Implementation(const EWeaponSlotType WeaponSlotType)
{
}

void UWeaponInventoryComponent::RemoveWeaponFromSlot_Implementation(const EWeaponSlotType WeaponSlotType)
{
	for (FWeaponSlot& slot : WeaponSlotData) {
		if (slot.SlotType == WeaponSlotType) {
			/*if (AttachedWeapon->GetWeaponData() == slot.WeaponInstance) {
				AttachedWeapon->SetWeaponData(nullptr);
			}*/
			slot.WeaponInstance = nullptr;
			
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
		UE_LOG(LogTemp, Warning, TEXT("No weapon instance in %s slot"), *str);
		OnWeaponChanged_Multicast(false, nullptr);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("switch current weapon slot?"));
	SwitchCurrentWeapon(wi, FindWeaponBySlot(ActivatingSlot), UseBlueprintBindFunction);
}

void UWeaponInventoryComponent::ClearWeaponSlotData(const EWeaponSlotType WeaponSlot)
{

	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to clear weapon with None enum type. ");
}

#pragma endregion
bool UWeaponInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool out = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (FWeaponSlot& data : WeaponSlotData) {
		Channel->ReplicateSubobject(data.WeaponInstance, *Bunch, *RepFlags);
	}
	return out;
}

void UWeaponInventoryComponent::OnWeaponChanged_Server_Implementation(bool SwitchWeaponSuccess, UWeaponInstance* ReplacementInstance)
{
	OnWeaponChanged_Multicast(SwitchWeaponSuccess, ReplacementInstance);
}

void UWeaponInventoryComponent::OnWeaponChanged_Multicast_Implementation(bool SwitchWeaponSuccess, UWeaponInstance* ReplacementInstance)
{
	OnWeaponEquip.Broadcast(SwitchWeaponSuccess, ReplacementInstance);
}



