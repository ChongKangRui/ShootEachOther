// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShootEachOtherPlayerController.generated.h"

class UInputMappingContext;
class USEOAbilitySystemComponent;
class ASEO_PlayerState;
/**
 *
 */
UCLASS()
class SHOOTEACHOTHER_API AShootEachOtherPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	USEOAbilitySystemComponent* GetSEOAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	ASEO_PlayerState* GetSEOPlayerState() const;
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void OnPossess(APawn* PawnToProcess) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	// End Actor interface
};
