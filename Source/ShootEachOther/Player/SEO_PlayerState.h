// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "SEO_PlayerState.generated.h"


class USEOAbilitySystemComponent;

UCLASS()
class SHOOTEACHOTHER_API ASEO_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASEO_PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	USEOAbilitySystemComponent* GetSEOAbilitySystemComponent() const { return AbilitySystemComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<const class USEO_AttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TObjectPtr<USEOAbilitySystemComponent> AbilitySystemComponent;
};
