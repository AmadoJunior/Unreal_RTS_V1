#include "Operator.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraMovementComponent.h"

// Constructor
AOperator::AOperator() {
	// Set Defaults
	DefaultZoomLength = 1350.0f;
	DefaultCameraRotation = FRotator(-75.0f, 0.0f, 0.0f); // Pitch(Y), Yaw(Z), Roll(X)

	// Root Component
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->InitSphereRadius(32.0f);
	CollisionSphere->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));

	// TO-DO: The collision profile name used for map boundaries

	// Default Settings for Inheriting Controller Rotations
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create Camera Arm & Attach to Root
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraArm->TargetArmLength = DefaultZoomLength;
	CameraArm->SetRelativeRotation(DefaultCameraRotation);
	CameraArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraArm->bEnableCameraLag = true; // Smooth Movements
	CameraArm->bEnableCameraRotationLag = false;
	CameraArm->bInheritPitch = false; //Needs to be false for zoom to update in real time

	// Create a camera...
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Ticks Every Frame
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Attach Movement Component
	MovementComponent = CreateDefaultSubobject<UCameraMovementComponent>(TEXT("CameraMovementComponent"));

}

// Called every frame
void AOperator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


// Called when the game starts or when spawned
void AOperator::BeginPlay() {
	Super::BeginPlay();
}

// Getters
float AOperator::GetCurrentArmLength() {
	return CameraArm->TargetArmLength;
}

FRotator AOperator::GetCurrentRotation() {
	return CameraArm->GetTargetRotation();
}

// Setters
void AOperator::SetArmLength(float ChangeAmount) {
	CameraArm->TargetArmLength += ChangeAmount;
}

void AOperator::SetArmRotation(FRotator ChangeAmount) {
	const FRotator RotationMax = FRotator(-25.0f, 0.0f, 0.0f); //Zoom In Rotation Max
	const FRotator RotationMin = DefaultCameraRotation; //Zoom Out Rotation Min (-75.0, 0.0, 0.0)

	// Get "X", The Rotation Change
	FRotator NewRotation = FRotator(CameraArm->GetTargetRotation() + ChangeAmount);

	//Clamp The Pitch of NewRotation
	NewRotation = NewRotation.Pitch < RotationMin.Pitch ? RotationMin : NewRotation.Pitch < RotationMax.Pitch ? NewRotation : RotationMax;

	CameraArm->SetRelativeRotation(NewRotation);
}

void AOperator::SetToDefaultZoom() {
	CameraArm->TargetArmLength = DefaultZoomLength;
	CameraArm->SetRelativeRotation(DefaultCameraRotation);
}