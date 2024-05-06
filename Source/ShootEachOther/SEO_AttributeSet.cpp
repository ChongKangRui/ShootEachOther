// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_AttributeSet.h"
#include "Net/UnrealNetwork.h"

USEO_AttributeSet::USEO_AttributeSet() : Health(100.0f)
{
}

void USEO_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USEO_AttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
}

void USEO_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USEO_AttributeSet, Health, OldHealth);
}

void USEO_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USEO_AttributeSet, MaxHealth, OldHealth);

	UE_LOG(LogTemp, Warning, TEXT("ONReppppppp Max health"));
}
