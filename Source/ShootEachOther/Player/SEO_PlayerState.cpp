// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_PlayerState.h"
#include "GameplayTagCollection.h"
#include "Player/SEO_PlayerComponent.h"
#include "SEO_AttributeSet.h"
#include "SEO_GlobalFunctionLibrary.h"
#include "GameMode/ShootEachOtherGameMode.h"
#include "Net/UnrealNetwork.h"


ASEO_PlayerState::ASEO_PlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<USEOAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USEO_AttributeSet>(TEXT("AttributeSet"));
}

void ASEO_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->AbilityActorInfo.Get()->OwnerActor = this;

	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode()) {
		sGM = Cast<AShootEachOtherGameMode>(GM);
	}

}

void ASEO_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Ready);
}

UAbilitySystemComponent* ASEO_PlayerState::GetAbilitySystemComponent() const
{
	return GetSEOAbilitySystemComponent();
}

bool ASEO_PlayerState::GetIsReady() const
{
	return Ready;
}

void ASEO_PlayerState::SetIsReady_Implementation(const bool IsReady)
{
	Ready = IsReady;

	if (Ready) {
		if (sGM)
			sGM->BeginNewRound();
	}

	

}

void ASEO_PlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;
}

void ASEO_PlayerState::ResetStatus_Implementation()
{
	if (HasAuthority()) {
		auto GameplayEffectClass = StaticLoadClass(UGameplayEffect::StaticClass(), nullptr, TEXT("/Game/Blueprint/GameplayAbilitySystem/GameplayEffect/GE_ResetHealth.GE_ResetHealth_C"));
		FGameplayEffectSpecHandle DamageEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1, AbilitySystemComponent->MakeEffectContext());

		if (!GameplayEffectClass) {
			UE_LOG(LogTemp, Error, TEXT("Invalid Gameplay Effect Class."));
			return;
		}
		if (DamageEffectHandle.IsValid()) {
			//We wan to set the damage during runtime based on weapon data from datatable.
			FGameplayEffectSpec& spec = *DamageEffectHandle.Data.Get();
			spec.SetSetByCallerMagnitude(GameplayTagsCollection::GameplayEvent_Reset, AttributeSet->MaxHealth.GetCurrentValue());
			//Finally we apply gameplay effect to target
			UE_LOG(LogTemp, Error, TEXT("apply gameplay effect success"));
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(spec);
		}
	}
	if (GetOwningController()) {
		if (GetOwningController()->IsLocalPlayerController()) {
			AbilitySystemComponent->RemoveLooseGameplayTag(GameplayTagsCollection::Status_Death);
			AbilitySystemComponent->RemoveLooseGameplayTag(GameplayTagsCollection::TAG_Gameplay_AbilityInputBlocked);
		}
	}
}

FGenericTeamId ASEO_PlayerState::GetGenericTeamId() const
{
	return TeamId;
}

int32 ASEO_PlayerState::GetTeamID() const
{
	return TeamId;
}

int ASEO_PlayerState::GetOwningMoney() const
{
	return OwningMoney;
}

bool ASEO_PlayerState::DeductOwningMoney(const int value)
{
	if (OwningMoney - value >= 0) {
		OwningMoney -= value;
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Not enough money to deduct"));
	return false;
}

bool ASEO_PlayerState::AddOwningMoney(const int value)
{
	OwningMoney += value;
	return true;
}



