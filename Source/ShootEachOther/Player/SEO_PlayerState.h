// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "SEO_PlayerState.generated.h"


//class USEOAbilitySystemComponent;

UCLASS()
class SHOOTEACHOTHER_API ASEO_PlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASEO_PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*Ability system*/
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	USEOAbilitySystemComponent* GetSEOAbilitySystemComponent() const { return AbilitySystemComponent; }

	/*Team related*/
	UFUNCTION(BlueprintPure, Category = "Team")
	bool GetIsReady() const;
	UFUNCTION(BlueprintCallable,Server, Reliable, Category = "Team")
	void SetIsReady(const bool IsReady);

	/*Shop System*/
	UFUNCTION(BlueprintPure, Category = "Shop System")
	int GetOwningMoney() const;
	/*Failed if money goes below 0 after deduct the value*/
	UFUNCTION(BlueprintCallable, Category = "Shop System")
	bool DeductOwningMoney(const int value);
	UFUNCTION(BlueprintCallable, Category = "Shop System")
	bool AddOwningMoney(const int value);

	UFUNCTION(BlueprintPure, Category = "Team")
	int32 GetTeamID() const;
	
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	FGenericTeamId TeamId;

	UPROPERTY(Replicated)
	bool Ready = false;

	UPROPERTY()
	TObjectPtr<const class USEO_AttributeSet> AttributeSet;

	int OwningMoney = 2000;
private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	TObjectPtr<USEOAbilitySystemComponent> AbilitySystemComponent;
	TObjectPtr<class AShootEachOtherGameMode> sGM;
	
};
