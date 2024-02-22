// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfig.h"


const UInputAction* UInputConfig::FindNativeActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FSInputAction& Action : NativeActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UInputConfig::FindAbilityActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FSInputAction& Action : AbilityActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}


