// Fill out your copyright notice in the Description page of Project Settings.


#include "SEOAbilitySystemComponent.h"
#include "GameplayTagCollection.h"
#include "SEO_GameplayAbility.h"


USEOAbilitySystemComponent::USEOAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void USEOAbilitySystemComponent::ProcessAllAbility(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(GameplayTagsCollection::TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilityToActivate;
	AbilityToActivate.Reset();

	for (FGameplayAbilitySpecHandle HeldSpecHandle : InputHeldSpecHandles) {
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(HeldSpecHandle)) {
			if (AbilitySpec->Ability && !AbilitySpec->IsActive()) {

				const USEO_GameplayAbility* Ability = CastChecked<USEO_GameplayAbility>(AbilitySpec->Ability);
				if (Ability->GetActivationPolicy() == EAbilityActivationPolicy::WhenTriggerActivating) {
					AbilityToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (FGameplayAbilitySpecHandle PressedSpecHandle : InputPressedSpecHandles) {
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(PressedSpecHandle)) {
			if (AbilitySpec->Ability) {

				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive()) {
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else {
					const USEO_GameplayAbility* Ability = CastChecked<USEO_GameplayAbility>(AbilitySpec->Ability);
					if (Ability->GetActivationPolicy() == EAbilityActivationPolicy::WhenTriggered) {
						AbilityToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}
	
	for (const FGameplayAbilitySpecHandle& ability : AbilityToActivate) {
		TryActivateAbility(ability);
	}


	for (FGameplayAbilitySpecHandle ReleasedSpecHandle : InputReleasedSpecHandles) {
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(ReleasedSpecHandle)) {
			if (AbilitySpec->Ability) {

				AbilitySpec->InputPressed = false;
				
				if (AbilitySpec->IsActive()) {
					UE_LOG(LogTemp, Display, TEXT("MArk as release"));
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}
	
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();

}

void USEOAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USEOAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USEOAbilitySystemComponent::ClearAbilityInput()
{
	UE_LOG(LogTemp, Display, TEXT("Clear All Ability"));
	InputPressedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void USEOAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}


void USEOAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				//UE_LOG(LogTemp, Display, TEXT("Released!!!!!!!!!!!!!"));
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}
