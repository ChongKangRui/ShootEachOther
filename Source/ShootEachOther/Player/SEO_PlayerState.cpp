// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_PlayerState.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"

#include "Player/SEO_PlayerComponent.h"
#include "SEO_AttributeSet.h"
#include "SEO_GlobalFunctionLibrary.h"


ASEO_PlayerState::ASEO_PlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<USEOAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USEO_AttributeSet>(TEXT("AttributeSet"));
	

}

UAbilitySystemComponent* ASEO_PlayerState::GetAbilitySystemComponent() const
{
	return GetSEOAbilitySystemComponent();
}

void ASEO_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->AbilityActorInfo.Get()->OwnerActor = this;

}


