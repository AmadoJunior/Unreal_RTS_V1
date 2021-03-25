#include "CameraMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

//Constructor
UCameraMovementComponent::UCameraMovementComponent()
{
	// Tick Every Frame
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	//Defaults
	bDisableCameraMovement = false;
	DefaultMovementSpeed = 15.0;
	MovementSpeedModifier = 1.0;
	DefaultZoomLength = 800.0f;
	DeltaArm = 150.0;
	DeltaRotation = FRotator(7.15, 0.0, 0.0);
	MaxZoom = 300.0;
	MinZoom = 3000.0;
}


// Called when the game starts
void UCameraMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//Populates Owner
	CameraOwner = Cast<AOperator>(GetOwner());
	//Populates PlayerControllerRef
	PlayerControllerRef = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}


// Called every frame
void UCameraMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//Getters
FVector UCameraMovementComponent::GetCameraLocation() {
	return CameraOwner->GetActorLocation();
}

FVector UCameraMovementComponent::GetFaceDirection() {
	return CameraOwner->GetActorForwardVector();
}
FRotator UCameraMovementComponent::GetCameraRotation() {
	return CameraOwner->GetActorRotation();
}

//Setters
float UCameraMovementComponent::SetMovementSpeedModifier(float ModifierAmount) {
	return MovementSpeedModifier = ModifierAmount;
}

float UCameraMovementComponent::SetMovementSpeed(float MovementSpeedAdjustment) {
	return DefaultMovementSpeed = MovementSpeedAdjustment;
}

bool UCameraMovementComponent::SetCameraDisable(bool bDisableCamera) {
	return bDisableCameraMovement = bDisableCamera;
}

//Utility
//Basic Movement Controls
// Movement Control - W A S D
void UCameraMovementComponent::BasicMovementControl(float AxisValueX, float AxisValueY) {
	// Set Variables used in movement calculation
	float MovementValueX;
	float MovementValueY;
	FVector MovementX;
	FVector MovementY;

	//If Camera !Disabled
	if (!IsCameraDisable()) {
		//Input On X && Y Axis
		if (AxisValueX != 0 && AxisValueY != 0) {
			MovementValueX = AxisValueX * GetCurrentMovementSpeed() * GetSpeedModifier();
			MovementX = FVector(MovementValueX, 0, 0);
			MovementValueY = AxisValueY * GetCurrentMovementSpeed() * GetSpeedModifier();
			MovementY = FVector(0, MovementValueY, 0);
			//MovementX + MovementY => Diagonal Movement
			CameraOwner->AddActorLocalOffset(MovementX + MovementY, true);
		}
		//If Input on W and S (Forward, Backward)
		else if (AxisValueX != 0) {
			//Movement Along X Axis
			MovementValueX = AxisValueX * GetCurrentMovementSpeed() * GetSpeedModifier();
			MovementX = FVector(MovementValueX, 0, 0);
			
			CameraOwner->AddActorLocalOffset(MovementX, true);
		}
		//If Input on A and D (Left, Right);
		else if (AxisValueY != 0) {
			MovementValueY = AxisValueY * GetCurrentMovementSpeed() * GetSpeedModifier();
			MovementY = FVector(0, MovementValueY, 0);

			CameraOwner->AddActorLocalOffset(MovementY, true);
		}
	}
}

void UCameraMovementComponent::PanCamera(float RotationAmount) {
}

void UCameraMovementComponent::ResetPan() {
}

void UCameraMovementComponent::EdgeScroll() {
}

void UCameraMovementComponent::ZoomIn() {
}

void UCameraMovementComponent::ZoomOut() {
}

void UCameraMovementComponent::DefaultZoom() {
}
