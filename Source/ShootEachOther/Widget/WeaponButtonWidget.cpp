// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponButtonWidget.h"
#include "Character/WeaponInventoryComponent.h"

void UWeaponButtonWidget::BuyWeapon(const EWeaponType& WeaponToGrant)
{
	if (const APlayerController* pc = GetOwningPlayer()) {
		if (UWeaponInventoryComponent* wic = pc->GetPawn()->GetComponentByClass<UWeaponInventoryComponent>()) {
			if (pc->HasAuthority())
				wic->AddWeaponToSlot(WeaponToGrant, true);
			else
				wic->AddWeaponToSlot_Server(WeaponToGrant, true);
		}
	
	}
	
}
