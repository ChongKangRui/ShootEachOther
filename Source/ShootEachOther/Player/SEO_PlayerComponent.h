// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagCollection.h"
#include "SEO_PlayerComponent.generated.h"


class USEO_AttributeSet;
class USEOAbilitySystemComponent;
class USEOPawnData;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTEACHOTHER_API USEO_PlayerComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	USEO_PlayerComponent();

	/*Use this to set whether the defaults binding(move and look) are allow to control or not*/
	UFUNCTION(BlueprintCallable)
	void SetNativeInputEnable(bool Enable = true);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData", Replicated)
	TObjectPtr<const USEOPawnData> PawnData;

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void InitializeInputContext();

	void InitializeInputBinding(UInputComponent* IC);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);

	void Input_AbilityTagPressed(FGameplayTag tag);
	void Input_AbilityTagReleased(FGameplayTag tag);

protected:
	TObjectPtr<APawn> m_Pawn;
	bool DisableNativeInput;

	friend class AShootEachOtherPlayerController;
	

};
