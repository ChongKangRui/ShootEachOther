// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "SEO_PlayerState.generated.h"


class USEOAbilitySystemComponent;

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


	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	/*Shop System*/
	UFUNCTION(BlueprintPure, Category = "Shop System")
	int GetOwningMoney() const;
	/*Failed if money goes below zero*/
	UFUNCTION(BlueprintCallable, Category = "Shop System")
	bool DeductOwningMoney(const int value);
	UFUNCTION(BlueprintCallable, Category = "Shop System")
	bool AddOwningMoney(const int value);

	UFUNCTION(BlueprintPure, Category = "Team")
	int32 GetTeamID() const;

protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY()
	TObjectPtr<const class USEO_AttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly)
	FGenericTeamId TeamId;

	int OwningMoney = 2000;

	UPROPERTY(Replicated)
	bool Ready = false;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TObjectPtr<USEOAbilitySystemComponent> AbilitySystemComponent;

	 TObjectPtr<class AShootEachOtherGameMode> sGM;
private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
