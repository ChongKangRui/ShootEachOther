// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_GlobalFunctionLibrary.h"



void USEO_GlobalFunctionLibrary::SEO_Log(const AActor* actor, const ELogType type, FString message)
{
	if (!actor) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Actor"));
		return;
	}

	FString authorityType;
	FString name = actor->GetName();

	if (actor->HasAuthority()) {
		authorityType = TEXT("[Server] ");
	}
	else {
		authorityType = TEXT("[Client] ");
	}

	switch (type) {
	case ELogType::Info:
		UE_LOG(LogTemp, Display, TEXT("%s %s: %s"), *authorityType, *name ,*message);
		break;
	case ELogType::Warning:
		UE_LOG(LogTemp, Warning, TEXT("%s %s: %s"), *authorityType, *name, *message);
		break;
	case ELogType::Error:
		UE_LOG(LogTemp, Error, TEXT("%s %s: %s"), *authorityType, *name, *message);
		break;
	}
}
