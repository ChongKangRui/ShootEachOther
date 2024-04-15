// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/SGA_MeleeSlash.h"
#include "GameplayTagCollection.h"

#include "Character/ShootEachOtherCharacter.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"

#include "Weapon/WeaponInstance.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponData.h"

#include "GameMode/GameState/SEO_GameState.h"



void USGA_MeleeSlash::MeleeTrace()
{

	if (const UWeaponInstance* wi = GetEquippedWeaponInstance()) {
		if (const AShootEachOtherCharacter* owner = GetSEOCharacter()) {
			FWeaponData data = wi->GetDefaultsWeaponData();
			FVector CameraVector = owner->GetFirstPersonCameraManager()->GetCameraLocation();
			FVector ForwardVector = CameraVector + (owner->GetFirstPersonCameraManager()->GetActorForwardVector() * data.TraceSphereForwardDistance);

			TArray<FHitResult> HitResults;

			FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(MeleeTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetAvatarActorFromActorInfo());

			/*ECC_GameTraceChannel2 is weapon trace. Can check DefaultEngine.ini for more detail*/
			const ECollisionChannel TraceChannel = ECC_GameTraceChannel2;

			GetWorld()->SweepMultiByChannel(HitResults, ForwardVector, ForwardVector, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(data.TraceSphereRadius), TraceParams);

			if (DebugDuration > 0.0f) {

				DrawDebugSphere(GetWorld(), ForwardVector, data.TraceSphereRadius, 32,FColor::Red, false, DebugDuration, 0, LineThickness);

			}

			TArray<FHitResult> EnemyHitResult;
			if (HitResults.Num() > 0)
			{
				// Filter the output list to prevent multiple hits on the same actor;
				// this is to prevent a single bullet dealing damage multiple times to
				// a single actor if using an overlap trace
				for (FHitResult& CurHitResult : HitResults)
				{
					auto Pred = [&CurHitResult](const FHitResult& Other)
						{
							return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
						};

					if (!EnemyHitResult.ContainsByPredicate(Pred))
					{
						EnemyHitResult.Add(CurHitResult);
						/*Apply damage here*/
						ApplyDamageToTarget(data.Damage, CurHitResult.GetActor());
						UE_LOG(LogTemp, Error, TEXT("Hit Name: %s"), *CurHitResult.GetActor()->GetName());
						if (DebugDuration > 0.0f)
							DrawDebugPoint(GetWorld(), CurHitResult.ImpactPoint, HitPointThickness, FColor::Green, false, DebugDuration);
					}
				}

			}

		}
	}
}

