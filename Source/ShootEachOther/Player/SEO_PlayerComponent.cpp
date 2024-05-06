// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SEO_PlayerComponent.h"
#include "Input/SEO_EnhancedInputComponent.h"
#include "Input/InputConfig.h"
#include "InputActionValue.h"
#include "Character/SEOPawnData.h"
#include "Character/ShootEachOtherCharacter.h"
#include "Camera/CameraComponent.h"

#include "GameplayAbility/SEO_AbilitySet.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"

#include "EnhancedInputSubsystems.h"
#include "SEO_PlayerState.h"

#include "Net/UnrealNetwork.h"
#include "SEO_GlobalFunctionLibrary.h"

// Sets default values for this component's properties
USEO_PlayerComponent::USEO_PlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	SetIsReplicated(true);
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USEO_PlayerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, PawnData, COND_InitialOnly);
}

void USEO_PlayerComponent::InitializeInputBinding(UInputComponent* IC)
{
	if (!IC) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Actor Input Component"));
		return;
	}
	if (const USEOPawnData* Data = PawnData) {
		if (const UInputConfig* Config = Data->InputSet) {
			if (USEO_EnhancedInputComponent* EIC = Cast<USEO_EnhancedInputComponent>(IC)) {

				
				EIC->BindNativeAction(Config, GameplayTagsCollection::Input_Move, ETriggerEvent::Triggered, this, &USEO_PlayerComponent::Input_Move, true);
				EIC->BindNativeAction(Config, GameplayTagsCollection::Input_Look, ETriggerEvent::Triggered, this, &USEO_PlayerComponent::Input_Look, true);
				
				TArray<uint32> Handle;
				EIC->BindAbilityActions(Config, this, &USEO_PlayerComponent::Input_AbilityTagPressed, &USEO_PlayerComponent::Input_AbilityTagReleased, Handle);
				
			}
		}

	}


}

void USEO_PlayerComponent::InitializeInputContext_Implementation()
{
	m_Pawn = Cast<AShootEachOtherCharacter>(GetOwner());

	if (m_Pawn) {
		if (APlayerController* PlayerController = Cast<APlayerController>(m_Pawn->Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				if (!PawnData) {
					USEO_GlobalFunctionLibrary::SEO_Log(m_Pawn, ELogType::Error, "Invalid Pawn data");
				}

				Subsystem->AddMappingContext(PawnData->InputSet->InputMappingContext, 0);

				if (!m_Pawn->InputComponent && m_Pawn->IsLocallyControlled()) {
					USEO_GlobalFunctionLibrary::SEO_Log(m_Pawn, ELogType::Error, "Create Input system due to invalid input component");
					m_Pawn->CreateInputComponent(USEO_EnhancedInputComponent::StaticClass());
				}

				InitializeInputBinding(m_Pawn->InputComponent);
			}
		}
		else {
			USEO_GlobalFunctionLibrary::SEO_Log(m_Pawn, ELogType::Error, "Invalid Controller");
		}
	}

}

void USEO_PlayerComponent::Input_Move(const FInputActionValue& Value)
{

	if (!m_Pawn)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (m_Pawn->Controller != nullptr && !DisableNativeInput)
	{
		m_Pawn->AddMovementInput(m_Pawn->GetActorForwardVector(), MovementVector.Y);
		m_Pawn->AddMovementInput(m_Pawn->GetActorRightVector(), MovementVector.X);
	}
}

void USEO_PlayerComponent::Input_Look(const FInputActionValue& Value)
{
	if (!m_Pawn)
		return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (m_Pawn->Controller != nullptr && !DisableNativeInput)
	{
		m_Pawn->AddControllerYawInput(LookAxisVector.X);
		m_Pawn->AddControllerPitchInput(LookAxisVector.Y);
	}


}

void USEO_PlayerComponent::Input_AbilityTagPressed(FGameplayTag tag)
{

	if (!m_Pawn) {
		UE_LOG(LogTemp, Display, TEXT("Invalid Pawn"));
		return;
	}
	const APlayerState* playerState = m_Pawn->GetPlayerState();
	if (!playerState) {
		UE_LOG(LogTemp, Display, TEXT("Invalid Player State"));
		return;
	}
	const ASEO_PlayerState* seoplayerState = Cast<ASEO_PlayerState>(playerState);
	if (!seoplayerState) {
		UE_LOG(LogTemp, Display, TEXT("Invalid SEO Player State"));
		return;
	}

	if (const auto asc = seoplayerState->GetSEOAbilitySystemComponent()) {
		asc->AbilityInputTagPressed(tag);
	}
}

void USEO_PlayerComponent::Input_AbilityTagReleased(FGameplayTag tag)
{
	if (!m_Pawn) {
		UE_LOG(LogTemp, Display, TEXT("Invalid Pawn"));
		return;
	}
	const APlayerState* playerState = m_Pawn->GetPlayerState();
	if (!playerState) {
		UE_LOG(LogTemp, Display, TEXT("Invalid Player State"));
		return;
	}
	const ASEO_PlayerState* seoplayerState = Cast<ASEO_PlayerState>(playerState);
	if (!seoplayerState) {
		UE_LOG(LogTemp, Display, TEXT("Invalid SEO Player State"));
		return;
	}

	if (const auto asc = seoplayerState->GetSEOAbilitySystemComponent()) {
		asc->AbilityInputTagReleased(tag);
	}

}

void USEO_PlayerComponent::SetNativeInputEnable(bool Enable)
{
	DisableNativeInput = !Enable;
}



