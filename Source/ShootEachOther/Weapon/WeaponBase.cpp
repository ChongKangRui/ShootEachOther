// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponInstance.h"

// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DefaultsSceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("DefaultsSceneRoot"));
	ShootingPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ShootingPoint"));
	ShootingPoint->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	HandIK_L = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("HandIK Left"));
	HandIK_L->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	ADSCameraPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ADS Camera Point"));
	ADSCameraPoint->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void AWeaponBase::SetWeaponData(UWeaponInstance* WI)
{
	WeaponInstance = WI;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USceneComponent* AWeaponBase::GetShootingPoint() const
{
	return ShootingPoint;
}

