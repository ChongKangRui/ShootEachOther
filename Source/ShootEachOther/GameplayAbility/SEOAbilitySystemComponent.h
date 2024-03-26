// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SEOAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USEOAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	USEOAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ProcessAllAbility(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintCallable)
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable)
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
};
