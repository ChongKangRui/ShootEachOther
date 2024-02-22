// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootEachOtherCharacter.h"
#include "../Weapon/ShootEachOtherProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Player/SEO_PlayerState.h"
#include "SEO_GlobalFunctionLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AShootEachOtherCharacter

AShootEachOtherCharacter::AShootEachOtherCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	//FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//TP_CameraTrace_L
	//FP_Camera
	//FirstPersonCameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,"FP_Camera");
	//FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	//FirstPersonCameraComponent->bUsePawnControlRotation = true;

	}

void AShootEachOtherCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

AShootEachOtherPlayerController* AShootEachOtherCharacter::GetSEOPlayerController() const
{
	return CastChecked<AShootEachOtherPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

ASEO_PlayerState* AShootEachOtherCharacter::GetSEOPlayerState() const
{
	return CastChecked<ASEO_PlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

USEOAbilitySystemComponent* AShootEachOtherCharacter::GetSEOAbilitySystemComponent() const
{
	return Cast<USEOAbilitySystemComponent>(GetAbilitySystemComponent());;
}

UAbilitySystemComponent* AShootEachOtherCharacter::GetAbilitySystemComponent() const
{
	const ASEO_PlayerState* ps = GetSEOPlayerState();
	if (ps) {
		return ps->GetAbilitySystemComponent();
	}
	USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "Invalid ability system component get from player state");
	return nullptr;
}

void AShootEachOtherCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AShootEachOtherCharacter::GetHasRifle()
{
	return bHasRifle;
}

APlayerCameraManager* AShootEachOtherCharacter::GetFirstPersonCameraManager() const
{
	if (const APlayerController* apc = Cast<APlayerController>(GetController())) {
		return apc->PlayerCameraManager;
	}
	return nullptr;
}


