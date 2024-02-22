// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/SGA_Aim.h"
#include "Character/WeaponInventoryComponent.h"
#include "Weapon/WeaponInstance.h"

void USGA_Aim::SetTraceState(const ETraceSourceType TraceType)
{
	if (UWeaponInstance* wi = GetEquippedWeaponInstance()) {
		wi->TraceType = TraceType;
	}
	
}
