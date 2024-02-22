// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SEO_GlobalFunctionLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ELogType : uint8 {
	Info, 
	Warning,
	Error
};


UCLASS()
class SHOOTEACHOTHER_API USEO_GlobalFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*Can difference between client and server, allow for more simple control*/
	UFUNCTION(BlueprintCallable)
	static void SEO_Log(const AActor* actor, const ELogType type ,FString message);
};
