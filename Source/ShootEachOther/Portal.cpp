// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ShootEachOtherPlayerController.h"
#include "Character/ShootEachOtherCharacter.h"
#include "Engine/GameViewportClient.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneRenderCapture2D"));
	SceneCaptureComponent->SetupAttachment(SceneRoot);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxDetection"));
	BoxComponent->SetupAttachment(SceneRoot);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APortal::OnEndOverlap);

}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSceneCamera();
	UpdateOverlapedCharacterTeleport();
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

	FVector Forward = ConvertPlayerDirectionToLinkedPortalDirection(RotatorXDir);
	FVector Right = ConvertPlayerDirectionToLinkedPortalDirection(RotatorYDir);
	FVector Up = ConvertPlayerDirectionToLinkedPortalDirection(RotatorZDir);

	FRotator WorldRotation = UKismetMathLibrary::MakeRotationFromAxes(Forward, Right, Up);
	//UKismetMathLibrary::MakeRotationFromAxes();
	LinkedPortal->GetSceneCaptureComp()->SetWorldLocationAndRotation(WorldLocation, WorldRotation);
}

void APortal::UpdateOverlapedCharacterTeleport()
{
	if (!LocalPlayer) {
		LocalPlayer = GetWorld()->GetFirstPlayerController<AShootEachOtherPlayerController>();
		UE_LOG(LogTemp, Error, TEXT("get controller ref portal"));
	}
	else {
		if (IsPlayerOverlapping) {
			if (IsPointCrossingPortal(LocalPlayer->PlayerCameraManager->GetCameraLocation(), GetActorLocation(), GetActorForwardVector())) {
				if (LocalPlayer->IsLocalController()) {
					UE_LOG(LogTemp, Error, TEXT("cannnn teleport"));
					TeleportPlayer();
				}
			}
		}
	}
}

void APortal::TeleportPlayer()
{
	//Teleport Position
	//Represent the opposite direction of this portal
	FTransform selfTransform = FTransform(GetActorRotation(), GetActorLocation(),
		FVector(GetActorScale().X * -1, GetActorScale().Y * -1, GetActorScale().Z));

	FVector InversePositionFromSelfToPlayer = selfTransform.InverseTransformPosition(LocalPlayer->GetPawn()->GetActorLocation());
	FVector TeleportLocation = LinkedPortal->GetActorTransform().TransformPosition(InversePositionFromSelfToPlayer);

	//Teleport Rotation
	FVector X, Y, Z;
	UKismetMathLibrary::GetAxes(LocalPlayer->GetPawn()->GetActorRotation(), X, Y, Z);
	FRotator TeleportRotation = UKismetMathLibrary::MakeRotationFromAxes(
		ConvertPlayerDirectionToLinkedPortalDirection(X),
		ConvertPlayerDirectionToLinkedPortalDirection(Y),
		ConvertPlayerDirectionToLinkedPortalDirection(Z));

	LocalPlayer->GetPawn()->SetActorLocationAndRotation(TeleportLocation, TeleportRotation);
	LocalPlayer->PlayerCameraManager->SetGameCameraCutThisFrame();

	//Control Rotation
	UKismetMathLibrary::GetAxes(LocalPlayer->GetControlRotation(), X, Y, Z);
	FRotator ControlRotation = UKismetMathLibrary::MakeRotationFromAxes(
		ConvertPlayerDirectionToLinkedPortalDirection(X),
		ConvertPlayerDirectionToLinkedPortalDirection(Y),
		ConvertPlayerDirectionToLinkedPortalDirection(Z));

	LocalPlayer->SetControlRotation(ControlRotation);
	
	UpdateVelocity();
}

void APortal::UpdateVelocity()
{
	FVector Velocity = LocalPlayer->GetPawn()->GetMovementComponent()->Velocity;
	FVector NewVelocity = ConvertPlayerDirectionToLinkedPortalDirection(Velocity.GetSafeNormal()) * Velocity.Length();
	LocalPlayer->GetPawn()->GetMovementComponent()->Velocity = NewVelocity;
}

APortal* APortal::GetLinkedPortal() const
{
	return LinkedPortal;
}

USceneCaptureComponent2D* APortal::GetSceneCaptureComp() const
{
	return SceneCaptureComponent;
}

void APortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == LocalPlayer->GetPawn()) {
		IsPlayerOverlapping = true;
	}
}

void APortal::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == LocalPlayer->GetPawn())
		IsPlayerOverlapping = false;
}

bool APortal::IsPointCrossingPortal(const FVector& CameraLocation, const FVector& PortalLocation, const FVector& PortalNormal)
{
	bool isPlayerInFront = IsPlayerInfront(CameraLocation, PortalLocation, PortalNormal);
	bool isIntersect = IsIntersect(CameraLocation, LastCameraLocation, PortalLocation, PortalNormal);

	bool isCrossing = isIntersect && !isPlayerInFront && LastPlayerInFront;

	LastPlayerInFront = isPlayerInFront;
	LastCameraLocation = CameraLocation;

	//UE_LOG(LogTemp, Error, TEXT("player infront: %s"), isPlayerInFront ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Error, TEXT("last player infront: %s"), LastPlayerInFront ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Error, TEXT("isIntersect: %s"), isIntersect ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Error, TEXT("isCrossing: %s"), isCrossing ? TEXT("True") : TEXT("False"));

	return isCrossing;
}

bool APortal::IsPlayerInfront(const FVector& CameraLocation, const FVector& PortalLocation, const FVector& PortalNormal)
{
	FVector Direction = CameraLocation - PortalLocation;
	FVector LastPlayerLocation;
	bool IsPlayerInFront = (FVector::DotProduct(PortalNormal, Direction.GetSafeNormal()) >= 0);
	return IsPlayerInFront;
}

bool APortal::IsIntersect(const FVector& CameraLocation, const FVector& lastCameraLocation, const FVector& PortalLocation, const FVector& PortalNormal)
{
	FPlane PortalPlane = UKismetMathLibrary::MakePlaneFromPointAndNormal(PortalLocation, PortalNormal);
	float T; FVector Intersection;
	bool IsIntersect = UKismetMathLibrary::LinePlaneIntersection(lastCameraLocation, CameraLocation, PortalPlane, T, Intersection);
	return IsIntersect;
}

FVector APortal::ConvertPlayerDirectionToLinkedPortalDirection(const FVector& Direction)
{
	FVector InverseDir = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Direction);

	FVector MirrorX = UKismetMathLibrary::MirrorVectorByNormal(InverseDir, FVector(1.0, 0.0, 0.0));
	FVector MirrorXY = UKismetMathLibrary::MirrorVectorByNormal(MirrorX, FVector(0.0, 1.0, 0.0));

	FVector Out = UKismetMathLibrary::TransformDirection(LinkedPortal->GetActorTransform(), MirrorXY);

	return Out;
}


	

