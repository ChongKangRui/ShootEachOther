// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_PlayerState.h"
//#include "GameplayAbility/SEOAbilitySystemComponent.h"

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



