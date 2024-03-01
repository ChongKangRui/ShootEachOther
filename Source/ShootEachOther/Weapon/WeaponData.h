// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SlateWrapperTypes.h"
#include "GameplayAbility/SEO_AbilitySet.h"
#include "WeaponData.generated.h"

class UAnimMontage;
class UNiagaraSystem;
class USoundBase;
class USEO_GameplayAbility;
class UUserWidget;
class AWeaponBase;
//struct FAbilitySet_GameplayAbility;

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
    /*Rifle/Machinegun*/
    Primary_Rifle_MP5,
    Primary_Rifle_Tavor_TAR_21,
    Primary_MachineGun_M249,
    /*Sniper*/
    Primary_Sniper_L42A1,
    Primary_Anti_Mat_Rifle,
    Primary_Slant_Rifle,
    /*Shotgun*/
    Primary_Silver_Sam,
    /*Pistol*/
    Secondary_Pistol,
    /*Resolver*/
    Secondary_Revolver,
    Secondary_Single_Action_Army_Revolver,
    Secondary_Fancy_Revolver,
    Secondary_Cavalry_Revolver,
    /*Melee*/
    Melee_DefaultsKnife,
    /*Grenade*/
    Grenade_ExplosiveGrenade,
    Grenade_FlashLight,
    Grenade_MolotovCocktail
};

UENUM(BlueprintType)
enum class ETraceSourceType : uint8 {
    ShootFromCameraToward,
    ShootFromWeaponFiringPointToward
};

UENUM(BlueprintType)
enum class EAimCameraType : uint8 {
    None,
    CameraTransformToADSPoint,
    /*If we have the widget*/
    UseWidgetAndHideWeaponAndPlayerBody
};


UENUM(BlueprintType)
enum class EWeaponSlotType : uint8 {
    /*For primary weapon, middle to big size of weapon, even SkullCrusher maybe?*/
    Primary = 0,
    /*For pistol, minor submachine gun etc*/
    Secondary = 1,
    /*Different type of melee, knife, small to middle size of melee*/
    Melee = 2,
    /*Just grenade, fireball, flashbang, Explosive Grenade*/
    Grenade = 3,
    /*Represent None which u shoundn't set it to be usually*/
    None
};

USTRUCT(BlueprintType)
struct SHOOTEACHOTHER_API FStackValue {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag Tag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 Value;

};



USTRUCT(BlueprintType)
struct SHOOTEACHOTHER_API FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance", Meta = (ToolTip = "Weapon Blueprint that gonna spawn and attach to character"))
    TSubclassOf<AWeaponBase> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance", Meta = (ToolTip = "Weapon Blueprint that gonna spawn and attach to character"))
    FName AttachName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Type", Meta = (ToolTip = "Damage Per Bullet"))
    EWeaponSlotType EWeaponSlotType = EWeaponSlotType::None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute", Meta = (ToolTip = "Damage Per Bullet"))
    float Damage = 0;

    /*Maximum shoot trace distance*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute", Meta = (ToolTip = "Damage Per Bullet"))
    float TraceDistance = 10000.0f;

    /*How fast the shooting rate*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute")
    float Rate = 1.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute")
    float Recoil = 0.1f;

    /*Consider shotgun and some special case*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute")
    int BulletPerShoot = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute")
    float ShopPrice = 1000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute")
    EAimCameraType AimingCameraType = EAimCameraType::None;

    /*When aiming with right mmouse button*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Trace Type")
    ETraceSourceType AimingTraceType = ETraceSourceType::ShootFromWeaponFiringPointToward;

    /*When not aimming with right mouse button*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Trace Type")
    ETraceSourceType NonAimingTraceType = ETraceSourceType::ShootFromCameraToward;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attribute", Meta = (ToolTip = "Setup Maximum Bullet per gun/clip "))
    TArray<FStackValue> StackValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Binding", Meta = (ToolTip = "Setup for ability, such as shoot, aim etc"))
    TArray<FAbilitySet_GameplayAbility> WeaponAbilitySet;
   
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop Appearance")
    TSoftObjectPtr<UObject> ImageTexture;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop Appearance")
    FVector2D size = {250, 100};

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta =( EditCondition = "AimingCameraType == EAimCameraType::UseWidgetAndHideWeaponAndPlayerBody"))
    TSubclassOf<UUserWidget> ReticleWidget;

    /*Hide the Black crosshair when aiming*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
    ESlateVisibility CrosshairVisibilityWhenAiming = ESlateVisibility::Hidden;

    /*The offset for damage reticle effect to set when aiming*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
    FVector2D DamageEffectOffsetWhenAiming;

    ///*Hide the Damage Reticle Effect*/
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
    //bool DisableDamageEffectWhenOwnReticleWidgetCreated;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
    UAnimMontage* EquipMontage;

   /* UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
    UAnimMontage* UnequipMontage;*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
    UAnimMontage* ShootMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
    UAnimMontage* ReloadMontage;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    //UNiagaraSystem* MuzzleFlash;
    //
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    //UNiagaraSystem* WeaponTracer;
    //
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    //UNiagaraSystem* ShellEjected;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
    USoundBase* ShootSFX;
    
};



