// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WeaponInventoryComponent.h"
#include "Character/ShootEachOtherCharacter.h"

#include "Engine/ActorChannel.h"
#include "Engine/DataTable.h"
#include "Weapon/WeaponInstance.h"
#include "Weapon/WeaponBase.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Player/ShootEachOtherPlayerController.h"
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
	DOREPLIFETIME(ThisClass, testWeaponInstance);
	

}

void UWeaponInventoryComponent::SwapWeaponInstance(const EWeaponSlotType ReplacementWeaponSlot, UWeaponInstance* Replacement)
{
	if (Replacement) {

		

		bool ShouldSwapToCurrentWeapon = ReplacementWeaponSlot == ActivatingSlot;
		/*Swap weapon for activating slot*/
		if (ShouldSwapToCurrentWeapon) {

			UWeaponInstance* pwi = FindWeaponBySlot(ActivatingSlot);
			SwitchCurrentWeapon(Replacement, pwi, false);
			return;

		}
		/*Swap weapon for non activating slot, so can just simply replace the reference with new weapon data*/
		else {
			
				// Find the index of the element to remove
				//for (int32 Index = 0; Index < WeaponSlotData.Num(); ++Index) {
				//	if (WeaponSlotData[Index]->GetDefaultsWeaponData().EWeaponSlotType == ReplacementWeaponSlot) {
				//		
				//		WeaponSlotData[Index] = Replacement;
				//		return;
				//	}
				//}
				//WeaponSlotData.AddUnique(Replacement);
				testWeaponInstance = Replacement;
				for (FWeaponSlot& ws : WeaponSlotData) {
					if (ws.SlotType == ReplacementWeaponSlot) {
						ws.WeaponInstance = Replacement;
						return;
					}
				}
		}

	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "SwapWeapon UnSuccess");
}

void UWeaponInventoryComponent::SetAttachedWeapon_Server_Implementation(AWeaponBase* passinWeapon)
{
	AttachedWeapon = passinWeapon;
}

//void UWeaponInventoryComponent::SwitchCurrentWeapon_Server_Implementation(UWeaponInstance* ReplacementInstance, UWeaponInstance* ToReplace, bool UseBlueprintBindFunction)
//{
//	SwitchCurrentWeapon_Client(ReplacementInstance, ToReplace, UseBlueprintBindFunction);
//}
//
//void UWeaponInventoryComponent::SwitchCurrentWeapon_Client_Implementation(UWeaponInstance* ReplacementInstance, UWeaponInstance* ToReplace, bool UseBlueprintBindFunction)
//{
//	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "multicast switch current weapon");
//	SwitchCurrentWeapon(ReplacementInstance, ToReplace, UseBlueprintBindFunction);
//}

void UWeaponInventoryComponent::SwitchCurrentWeapon_Implementation(UWeaponInstance* ReplacementInstance, UWeaponInstance* ToReplace, bool UseBlueprintBindFunction)
{

	if (AShootEachOtherCharacter* pawn = Cast<AShootEachOtherCharacter>(GetOwner())) {
		USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "tryinh to switch current weapon");
		if (const AShootEachOtherPlayerController* controller = Cast<AShootEachOtherPlayerController>(pawn->Controller)) {
			if (USEOAbilitySystemComponent* asc = controller->GetSEOAbilitySystemComponent()) {

				if (!ReplacementInstance) {
					USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Invalid replacement weapon instance reference");
					return;
				}

				if (ToReplace) {
					ToReplace->ClearAbilityFromASC(asc);
					AttachedWeapon->Destroy();
				}
				if(UseBlueprintBindFunction)
					OnWeaponEquip.Broadcast(true, ReplacementInstance);
				else {
					if (pawn->HasAuthority()) {
						USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Set Success");
						AttachedWeapon = ReplacementInstance->InitializeForWeapon(asc, pawn);
					}
					else {
						USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "apa lanjiao?");
					}
				}

				/*Make sure activating slot is alway the correct slot*/
				ActivatingSlot = ReplacementInstance->GetDefaultsWeaponData().EWeaponSlotType;
				USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Weapon Instance Switch success");
			}
			else
				USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Invalid asc");
		}
		else
			USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Invalid controller");
	}
	else
		USEO_GlobalFunctionLibrary::SEO_Log(pawn, ELogType::Error, "Invalid controller");
}
#pragma region Getter Function

UWeaponInstance* UWeaponInventoryComponent::FindWeaponBySlot(const EWeaponSlotType type)
{
	//for (UWeaponInstance* wi : WeaponSlotData) {
	//	if (!wi) {
	//		USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Invalid Weapon Instance but got a reference in array");
	//		continue;
	//	}
	//		if (wi->GetDefaultsWeaponData().EWeaponSlotType == type)
	//			return wi;
	//	
	//}
	for (FWeaponSlot& wi : WeaponSlotData) {
		if (wi.SlotType == type)
			return wi.WeaponInstance;
	}
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to find weapon with not exist enum type. ");
	return nullptr;
}

const UWeaponInstance* UWeaponInventoryComponent::FindWeaponBySlot(EWeaponSlotType type) const
{
	//for (const UWeaponInstance* wi : WeaponSlotData) {
	//	if (!wi) {
	//		USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Invalid Weapon Instance but got a reference in array");
	//		continue;
	//	}
	//	if (wi->GetDefaultsWeaponData().EWeaponSlotType == type)
	//		return wi;
	//}
	for (const FWeaponSlot& wi : WeaponSlotData) {
		if (wi.SlotType == type)
			return wi.WeaponInstance;
	}
	
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "You are trying to find weapon with not exist enum type. ");
	return nullptr;
}

void UWeaponInventoryComponent::OnRep_SlotData()
{
	UE_LOG(LogTemp, Error, TEXT("Replicccccccccccccc"));
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

	//for (const UWeaponInstance* weaponData : WeaponSlotData) {
	//	if (!weaponData) {
	//		USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Invalid Weapon Instance but got a reference in array");
	//		continue;
	//	}
	//	if (weaponData->GetDefaultsWeaponData().EWeaponSlotType == WeaponSlot) {
	//		
	//		return weaponData->GetDefaultsWeaponData();
	//	}
	//}
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
	AddWeaponToSlot_Server(EWeaponType::Secondary_Pistol);
	
	SetCurrentWeaponSlot(EWeaponSlotType::Secondary);

}

void UWeaponInventoryComponent::AddWeaponToSlot_Server_Implementation(const EWeaponType WeaponType, bool ReplaceWeapon)
{
	AddWeaponToSlot(WeaponType, ReplaceWeapon);
}

void UWeaponInventoryComponent::AddWeaponToSlot_MultiCast_Implementation(const EWeaponType WeaponType, bool ReplaceWeapon)
{
	AddWeaponToSlot(WeaponType, ReplaceWeapon);
}


void UWeaponInventoryComponent::SetCurrentWeaponSlot_Implementation(const EWeaponSlotType WeaponSlot, bool UseBlueprintBindFunction)
{
	if (WeaponSlot == ActivatingSlot) {
		UE_LOG(LogTemp, Warning, TEXT("Same Slot, no trigger happen"));
		OnWeaponEquip.Broadcast(false, nullptr);
		return;
	}
	UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot);
	
	if (!wi) {
		FString str = UEnum::GetValueAsString(WeaponSlot);
		USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "No weapon instance in slot");
		UE_LOG(LogTemp, Warning, TEXT("No weapon instance in %s slot"), *str);
		OnWeaponEquip.Broadcast(false, nullptr);
		return;
	}
	
	SwitchCurrentWeapon(wi, FindWeaponBySlot(ActivatingSlot), UseBlueprintBindFunction);
}

void UWeaponInventoryComponent::ClearWeaponSlotData(const EWeaponSlotType WeaponSlot)
{
	//for (int i = 0; i < WeaponSlotData.Num();i++) {
	//	if (WeaponSlotData[i]->GetDefaultsWeaponData().EWeaponSlotType == WeaponSlot) {
	//		WeaponSlotData.RemoveAt(i);
	//		return;
	//	}
	//}

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

	if (!ReplaceWeapon) {
		if (FindWeaponBySlot(data->EWeaponSlotType)) {
			UE_LOG(LogTemp, Error, TEXT("Weapon Slot aldy contain weapon data!!!"));
			return;
		}
	}
	//Create Weapon instance
	UWeaponInstance* WI = NewObject<UWeaponInstance>(GetOwner());
	WI->InitializeWeaponInstance(WeaponType,*data);
	
	SwapWeaponInstance(data->EWeaponSlotType, WI);
	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Warning, "Finished Weapon ADDed");
}

#pragma endregion

//#pragma region Weapon Stat Getter and Setter
///// Weapon Stat Related
//int32 UWeaponInventoryComponent::GetWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag) const
//{
//	if (const UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot)) {
//		return wi->GetStatCount(StatTag);
//	}
//	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Weapon Slot may not contain data to Get Stat!!!");
//	return -1;
//}
//
//int32 UWeaponInventoryComponent::GetCurrentWeaponStatCount(const FGameplayTag StatTag) const
//{
//	return GetWeaponStatCount(ActivatingSlot, StatTag);
//}
//
//void UWeaponInventoryComponent::AddWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag, const int32 value)
//{
//	if (UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot)) {
//		wi->AddStatCount(StatTag, value);
//	}
//	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Weapon Slot may not contain data to Add Stat!!!");
//}
//
//void UWeaponInventoryComponent::AddCurrentWeaponStatCount(const FGameplayTag StatTag, const int32 value)
//{
//	AddWeaponStatCount(ActivatingSlot, StatTag, value);
//}
//
//void UWeaponInventoryComponent::RemoveWeaponStatCount(const EWeaponSlotType WeaponSlot, const FGameplayTag StatTag, const int32 value)
//{
//	if (UWeaponInstance* wi = FindWeaponBySlot(WeaponSlot)) {
//		wi->RemoveStatCount(StatTag, value);
//	}
//	USEO_GlobalFunctionLibrary::SEO_Log(GetOwner(), ELogType::Error, "Weapon Slot may not contain data to Remove Stat!!!");
//}
//
//void UWeaponInventoryComponent::RemoveCurrentWeaponStatCount(const FGameplayTag StatTag, const int32 value)
//{
//	RemoveWeaponStatCount(ActivatingSlot, StatTag, value);
//}
//
//#pragma endregion
/// Weapon Stat End Related

bool UWeaponInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool out = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (FWeaponSlot& data : WeaponSlotData) {
		Channel->ReplicateSubobject(data.WeaponInstance, *Bunch, *RepFlags);
	}
	return out;
}



