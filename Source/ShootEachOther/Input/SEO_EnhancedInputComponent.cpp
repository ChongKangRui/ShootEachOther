// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_EnhancedInputComponent.h"

void USEO_EnhancedInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
