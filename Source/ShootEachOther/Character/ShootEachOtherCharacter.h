// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "ShootEachOtherCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USEOAbilitySystemComponent;
class ASEO_PlayerState;
class AShootEachOtherPlayerController;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
UCLASS(config=Game)
class AShootEachOtherCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* CharacterMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
public:
	AShootEachOtherCharacter();
public:
	UFUNCTION(BlueprintCallable, Category = "Character")
	AShootEachOtherPlayerController* GetSEOPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	ASEO_PlayerState* GetSEOPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	USEOAbilitySystemComponent* GetSEOAbilitySystemComponent() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
public:
	/** Returns FirstPersonCameraComponent subobject **/
	APlayerCameraManager* GetFirstPersonCameraManager() const;
};

