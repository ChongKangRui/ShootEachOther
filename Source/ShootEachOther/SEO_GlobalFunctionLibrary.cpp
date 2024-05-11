// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_GlobalFunctionLibrary.h"
#include "GameplayTagCollection.h"
#include "Character/ShootEachOtherCharacter.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "Weapon/WeaponInstance.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponData.h"
#include "SEO_AttributeSet.h"
#include "AI/AIBotController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/SEO_PlayerState.h"

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

void USEO_GlobalFunctionLibrary::ApplyDamageToTarget(const float Damage, TSubclassOf<UGameplayEffect> DamageGE, AActor* Causer, AActor* HitActor, bool HasFriendlyFire)
{
	/*Apply damage gameplay effect to target*/
	if (HitActor) {
		AShootEachOtherCharacter* Target = Cast<AShootEachOtherCharacter>(HitActor);
		AShootEachOtherCharacter* DamageSource = Cast<AShootEachOtherCharacter>(Causer);
		if (!Target || !DamageSource) {
			USEO_GlobalFunctionLibrary::SEO_Log(Causer, ELogType::Warning, "Invalid Damage Target/Causer");
			return;
		}

		if (USEOAbilitySystemComponent* TargetASC = Target->GetSEOAbilitySystemComponent()) {

			//Apply damage to target
			USEOAbilitySystemComponent* asc = DamageSource->GetSEOAbilitySystemComponent();
			FGameplayEffectSpecHandle DamageEffectHandle = asc->MakeOutgoingSpec(DamageGE, 1,asc->MakeEffectContext());

			if (!HasFriendlyFire) {
				if (Target->GetSEOPlayerState()->GetGenericTeamId() == DamageSource->GetSEOPlayerState()->GetGenericTeamId()) {
					USEO_GlobalFunctionLibrary::SEO_Log(Causer, ELogType::Warning, "Is the same id");
					return;
				}
			}

			if (DamageEffectHandle.IsValid() && asc) {
				//We wan to set the damage during runtime based on weapon data from datatable.
				FGameplayEffectSpec& spec = *DamageEffectHandle.Data.Get();
				spec.SetSetByCallerMagnitude(GameplayTagsCollection::WeaponDamage, -Damage);
				//Finally we apply gameplay effect to target
				asc->ApplyGameplayEffectSpecToTarget(spec, TargetASC);
				
				if (const USEO_AttributeSet* EnemyAttribute = Cast<USEO_AttributeSet>(TargetASC->GetAttributeSet(USEO_AttributeSet::StaticClass()))) {
					if (EnemyAttribute->GetHealth() <= 0) {
						Target->OnCharacterDeath();
						
						//Ask bot to clear target
						if (DamageSource->GetPlayerState()->IsABot()) {
							DamageSource->GetBotController()->GetBlackboardComponent()->ClearValue("Target");
						}
						else{
							DamageSource->GetSEOPlayerState()->AddOwningMoney(500);
						}
					}
				}
				return;
			}
		}


	}
}