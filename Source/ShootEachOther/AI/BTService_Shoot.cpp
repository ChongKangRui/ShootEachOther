// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Shoot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Character/ShootEachOtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagCollection.h"
#include "AIController.h"


UBTService_Shoot::UBTService_Shoot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NodeName = "Shoot";
}



void UBTService_Shoot::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);
    
}

void UBTService_Shoot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (!SelfCharacter) {
        SelfCharacter = Cast<AShootEachOtherCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(selfActor.SelectedKeyName));
       
    }
    if (!Enemy) {
        Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName));
    }

    FVector DirectionToTarget = Enemy->GetActorLocation() - SelfCharacter->GetActorLocation();
    FVector DirectionToTargetNormalize = DirectionToTarget.GetSafeNormal();
    
    float YawRadian = FMath::Atan2(DirectionToTargetNormalize.Y, DirectionToTargetNormalize.X);
    float YawAngle = FMath::RadiansToDegrees(YawRadian);

    FRotator TargetRotation = FRotator(0, YawAngle, 0.0f);
    FRotator FinalRotation = FMath::RInterpTo(SelfCharacter->Controller->GetControlRotation(),
        TargetRotation, DeltaSeconds, 100.0f);

    SelfCharacter->AIRotation = FinalRotation;

    if (SelfCharacter->GetSEOAbilitySystemComponent()) {
        UE_LOG(LogTemp, Error, TEXT("AIShoot"))
        SelfCharacter->GetSEOAbilitySystemComponent()->AbilityInputTagPressed(GameplayTagsCollection::Input_Shoot);
        SelfCharacter->GetSEOAbilitySystemComponent()->AbilityInputTagPressed(GameplayTagsCollection::Input_Shoot_Auto);
    }
}
