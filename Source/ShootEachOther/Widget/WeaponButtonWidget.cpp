// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponButtonWidget.h"
#include "Character/WeaponInventoryComponent.h"

void UWeaponButtonWidget::BuyWeapon(const EWeaponType& WeaponToGrant)
{
	if (const APlayerController* pc = GetOwningPlayer()) {
		if (UWeaponInventoryComponent* wic = pc->GetPawn()->GetComponentByClass<UWeaponInventoryComponent>()) {
			wic->AddWeaponToSlot(WeaponToGrant, true);
		}
	}
}
