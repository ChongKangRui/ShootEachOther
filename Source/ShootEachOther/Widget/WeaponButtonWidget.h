// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponData.h"
#include "WeaponButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API UWeaponButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void BuyWeapon(const EWeaponType& GrantedWeapon);



};
