// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Shoot.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTEACHOTHER_API UBTService_Shoot : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_Shoot(/*const FObjectInitializer& ObjectInitializer*/);

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector selfActor;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector Target;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	class AShootEachOtherCharacter* SelfCharacter;
	class AActor* Enemy;
};
