// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/SGA_Shoot.h"
#include "GameplayTagCollection.h"
#include "GameplayAbility/SEOAbilitySystemComponent.h"

#include "Weapon/WeaponInstance.h"
#include "Weapon/WeaponBase.h"

#include "Camera/CameraComponent.h"
#include "Character/ShootEachOtherCharacter.h"
#include "Character/WeaponInventoryComponent.h"

#include "Player/SEO_PlayerComponent.h"
#include "SEO_GlobalFunctionLibrary.h"




void USGA_Shoot::StartWeaponTrace()
{
	if (!DamageGE) {
		USEO_GlobalFunctionLibrary::SEO_Log(GetAvatarActorFromActorInfo(), ELogType::Error, "Invalid Damage Gameplay Effect, weapon logic wont continue");
		return;
	}

	if (const UWeaponInstance* wi = GetEquippedWeaponInstance()) {
		const AShootEachOtherCharacter* character = GetSEOCharacter();
		if (character && character->IsLocallyControlled()) {

			//For loop so it is flexible when come to shotgun circumstance
			for (int i = 0; i < wi->GetDefaultsWeaponData().BulletPerShoot; i++) {

				const FVector TraceStart = GetTraceStart(wi->TraceType);
				const FVector TraceDirection = GetTraceDirection(wi->TraceType, wi->GetDefaultsWeaponData().TraceDistance);
				


				TArray<FHitResult> results;
				FHitResult Hit = WeaponTrace(TraceStart, TraceDirection, results, 0.0f, DebugDuration);

				/*Apply damage*/
				ApplyDamageToTarget(wi->GetDefaultsWeaponData().Damage, Hit.GetActor());

				FString msg = FString("TraceDirection  == ") + TraceDirection.ToString();
				USEO_GlobalFunctionLibrary::SEO_Log(GetAvatarActorFromActorInfo(), ELogType::Error, msg);
				
				if(character->HasAuthority())
					OnWeaponFired({Hit});
				else {
					OnWeaponFired_Server({ Hit });
				}
			}

		}
		
	}
}

FHitResult USGA_Shoot::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, TArray<FHitResult>& OutHitResult, float SweepRadius, float DebugTime) const
{
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetAvatarActorFromActorInfo());
	
	/*ECC_GameTraceChannel2 is weapon trace. Can check DefaultEngine.ini for more detail*/
	const ECollisionChannel TraceChannel = ECC_GameTraceChannel2;
	
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	if (DebugTime > 0.0f) {
		
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, DebugTime, 0, LineThickness);
		
	}
	FHitResult Hit(ForceInit);
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

			if (!OutHitResult.ContainsByPredicate(Pred))
			{
				OutHitResult.Add(CurHitResult);
				if(DebugTime > 0.0f)
					DrawDebugPoint(GetWorld(), CurHitResult.ImpactPoint, HitPointThickness, FColor::Green, false, DebugTime);
			}
		}

		Hit = OutHitResult.Last();
	}
	else
	{
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
	


    
}

/*Replicated Hitresult is needed*/
void USGA_Shoot::OnWeaponFired_Server_Implementation(const TArray<FHitResult>& HitResult)
{
	OnWeaponFired_MultiCast(HitResult);
}

void USGA_Shoot::OnWeaponFired_MultiCast_Implementation(const TArray<FHitResult>& HitResult)
{
	OnWeaponFired(HitResult);
}

void USGA_Shoot::ApplyDamageToTarget(const float Damage,AActor* HitActor)
{
	/*Apply damage gameplay effect to target*/
	if (HitActor) {
		AShootEachOtherCharacter* Target = Cast<AShootEachOtherCharacter>(HitActor);
		if (!Target) {
			USEO_GlobalFunctionLibrary::SEO_Log(GetAvatarActorFromActorInfo(), ELogType::Warning, "Invalid Damage Target");
			return;
		}

		if (USEOAbilitySystemComponent* TargetASC = Target->GetSEOAbilitySystemComponent()) {

			//Apply damage to target
			FGameplayEffectSpecHandle DamageEffectHandle = MakeOutgoingGameplayEffectSpec(DamageGE);
			USEOAbilitySystemComponent* asc = GetSEOAbilitySystemComponent();
			if (DamageEffectHandle.IsValid() && asc) {
				//We wan to set the damage during runtime based on weapon data from datatable.
				FGameplayEffectSpec& spec = *DamageEffectHandle.Data.Get();
				spec.SetSetByCallerMagnitude(GameplayTagsCollection::WeaponDamage, -Damage);
				USEO_GlobalFunctionLibrary::SEO_Log(GetAvatarActorFromActorInfo(), ELogType::Info, "Apply gameplayeffect success");
				
				//Finally we apply gameplay effect to target
				asc->ApplyGameplayEffectSpecToTarget(spec, TargetASC);
				return;
			}
		}
	
		
	}
}

FVector USGA_Shoot::GetTraceStart(const ETraceSourceType& type) const
{
	switch (type) {
	case ETraceSourceType::ShootFromWeaponFiringPointToward: {
		const UWeaponInventoryComponent* wic = GetWeaponInventoryComponent();
		if (wic) {
			if (const AWeaponBase* AttachedWeapon = wic->GetCurrentAttachedWeapon()) {
				return AttachedWeapon->GetShootingPoint()->GetComponentLocation();
			}
		}
		UE_LOG(LogTemp, Error, TEXT("Invalid weapon inventory component or attached weapon reference !!!"));
	}
	return FVector();
	case ETraceSourceType::ShootFromCameraToward:
		const APlayerCameraManager* CameraManager = GetSEOCharacter()->GetFirstPersonCameraManager();
		return CameraManager->GetCameraLocation();

	}
	UE_LOG(LogTemp, Error, TEXT("Unknown ETraceSourceType !!!"));
	return FVector();
}

FVector USGA_Shoot::GetTraceDirection(const ETraceSourceType& type, const float& TraceDistance) const
{
	switch (type) {
	case ETraceSourceType::ShootFromWeaponFiringPointToward:{
		const UWeaponInventoryComponent* wic = GetWeaponInventoryComponent();
		if (wic) {
			if (const AWeaponBase* AttachedWeapon = wic->GetCurrentAttachedWeapon()) {
				UE_LOG(LogTemp, Warning, TEXT("firepoint Forward %s"), *AttachedWeapon->GetShootingPoint()->GetForwardVector().ToString());
				FVector WeaponPointForwardVector = AttachedWeapon->GetShootingPoint()->GetForwardVector();
				return AttachedWeapon->GetShootingPoint()->GetComponentLocation() + (WeaponPointForwardVector * TraceDistance);
			}
		}
		UE_LOG(LogTemp, Error, TEXT("Invalid weapon inventory component or attached weapon reference !!!"));
	}
		return FVector();
	case ETraceSourceType::ShootFromCameraToward:
		const APlayerCameraManager* CameraManager = GetSEOCharacter()->GetFirstPersonCameraManager();
		UE_LOG(LogTemp, Warning, TEXT("Cam Forward %s"), *CameraManager->GetActorForwardVector().ToString());
		FVector CamForwardVector = CameraManager->GetCameraRotation().Vector();
		return CameraManager->GetCameraLocation() + (CamForwardVector * TraceDistance);

	}
	UE_LOG(LogTemp, Error, TEXT("Unknown ETraceSourceType !!!"));
	return FVector();
}


FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		// consider the cone a concatenation of two rotations. one "away" from the center line, and another "around" the circle
		// apply the exponent to the away-from-center rotation. a larger exponent will cluster points more tightly around the center
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}
