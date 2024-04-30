// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ShootEachOtherPlayerController.h"
#include "Engine/GameViewportClient.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneRenderCapture2D"));
	SceneCaptureComponent->AttachToComponent(SceneRoot,FAttachmentTransformRules::KeepRelativeTransform);

	
}

APortal* APortal::GetLinkedPortal() const
{
	return LinkedPortal;
}

 USceneCaptureComponent2D* APortal::GetSceneCaptureComp() const
{
	return SceneCaptureComponent;
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSceneCamera();
}

void APortal::UpdateSceneCamera()
{
	if (!LinkedPortal) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Linked Portal"))
		return;
	}

	/*Location Calculation*/
	FTransform selfTransform = FTransform(GetActorRotation(), GetActorLocation(),
		FVector(GetActorScale().X * -1, GetActorScale().Y * -1, GetActorScale().Z));

	APlayerCameraManager* playerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector RelativeLocation = selfTransform.InverseTransformPosition(playerCamera->GetCameraLocation());
	FVector WorldLocation = LinkedPortal->GetActorTransform().TransformPosition(RelativeLocation);

	/*Rotation Calculation*/
	FVector RotatorXDir, RotatorYDir, RotatorZDir;
	UKismetMathLibrary::BreakRotIntoAxes(playerCamera->GetCameraRotation(), RotatorXDir, RotatorYDir, RotatorZDir);

	FVector Forward = ConvertPlayerVisionToLinkedPortalVision(RotatorXDir);
	FVector Right = ConvertPlayerVisionToLinkedPortalVision(RotatorYDir);
	FVector Up = ConvertPlayerVisionToLinkedPortalVision(RotatorZDir);

	FRotator WorldRotation = UKismetMathLibrary::MakeRotationFromAxes(Forward, Right, Up);
	//UKismetMathLibrary::MakeRotationFromAxes();
	LinkedPortal->GetSceneCaptureComp()->SetWorldLocationAndRotation(WorldLocation, WorldRotation);
}

FVector APortal::ConvertPlayerVisionToLinkedPortalVision(const FVector& Direction)
{
	FVector InverseDir = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Direction);

	FVector MirrorX = UKismetMathLibrary::MirrorVectorByNormal(InverseDir,FVector(1.0, 0.0, 0.0));
	FVector MirrorXY = UKismetMathLibrary::MirrorVectorByNormal(MirrorX, FVector(0.0, 1.0, 0.0));

	FVector Out = UKismetMathLibrary::TransformDirection(LinkedPortal->GetActorTransform(), MirrorXY);
	
	return Out;
}


	

