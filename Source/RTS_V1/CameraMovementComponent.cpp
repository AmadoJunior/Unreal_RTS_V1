#include "CameraMovementComponent.h"
#include "Operator.h"
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
	DefaultZoomLength = 1350.0f;
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

//Camera Pan
void UCameraMovementComponent::PanCamera(float RotationAmount) {
	FRotator NewRotation = GetCameraRotation().Add(0, RotationAmount, 0);
	CameraOwner->SetActorRotation(NewRotation);
}

//Resets Camara Pan/Rotation
void UCameraMovementComponent::ResetPan() {
	CameraOwner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

//Edge Scroll - X and Y are reversed on screen from the game world
void UCameraMovementComponent::EdgeScroll() {
	//Vars for Movement
	//DeltaSpeed => Change of Speed in Axis
	float DeltaSpeedX;
	float DeltaSpeedY;
	FVector MovementX; //Use with DeltaSpeedY
	FVector MovementY; //Use with DeltaSpeedX

	//Get Mouse Position
	float MousePositionX{ 0 };
	float MousePositionY{ 0 };
	PlayerControllerRef->GetMousePosition(MousePositionX, MousePositionY);

	//Get Viewport (play screen) size
	int32 ViewportSizeX;
	int32 ViewportSizeY;
	PlayerControllerRef->GetViewportSize(ViewportSizeX, ViewportSizeY);

	//Calculate Proportions (We want these calculations to be updated with the mouse movements)
	float ProportionX = MousePositionX / ViewportSizeX;
	float ProportionY = MousePositionY / ViewportSizeY;

	// Set EdgeScrollSpeeds Baded on Proportion
	if (ProportionX >= .975 && ProportionY <= .025) { //TOP RIGHT CORNER
		DeltaSpeedX = 7.07 * GetSpeedModifier();
		DeltaSpeedY = 7.07;

		//Flipped for Gameworld
		MovementX = FVector(DeltaSpeedY, 0.0f, 0.0f);
		MovementY = FVector(0.0f, DeltaSpeedX, 0.0f);

		CameraOwner->AddActorLocalOffset(MovementX + MovementY, true);
		SetCameraDisable(true);

	} else if (ProportionX >= .975 && ProportionY >= .975) { //BOTTOM RIGHT
		DeltaSpeedX = 7.07 * GetSpeedModifier();
		DeltaSpeedY = -7.07;

		//Flipped for Gameworld
		MovementX = FVector(DeltaSpeedY, 0.0f, 0.0f);
		MovementY = FVector(0.0f, DeltaSpeedX, 0.0f);

		CameraOwner->AddActorLocalOffset(MovementX + MovementY, true);
		SetCameraDisable(true);

	} else if (ProportionX <= .025 && ProportionY >= .975) { //BOTTOM LEFT
		DeltaSpeedX = -7.07 * GetSpeedModifier();
		DeltaSpeedY = -7.07;

		//Flipped for Gameworld
		MovementX = FVector(DeltaSpeedY, 0.0f, 0.0f);
		MovementY = FVector(0.0f, DeltaSpeedX, 0.0f);

		CameraOwner->AddActorLocalOffset(MovementX + MovementY, true);
		SetCameraDisable(true);

	} else if (ProportionX <= .025 && ProportionY <= .025) { //TOP LEFT
		DeltaSpeedX = -7.07 * GetSpeedModifier();
		DeltaSpeedY = 7.07;

		//Flipped for Gameworld
		MovementX = FVector(DeltaSpeedY, 0.0f, 0.0f);
		MovementY = FVector(0.0f, DeltaSpeedX, 0.0f);

		CameraOwner->AddActorLocalOffset(MovementX + MovementY, true);
		SetCameraDisable(true);

	} else {
		//Edges Done
		if (ProportionX >= .975) { //RIGHT
			DeltaSpeedX = 10.0f * GetSpeedModifier();
			DeltaSpeedY = 0.0f;

			MovementY = FVector(0.0f, DeltaSpeedX, 0.0f);

			CameraOwner->AddActorLocalOffset(MovementY, true);
			SetCameraDisable(true);
		}
		else if (ProportionX <= .025) { //LEFT
			DeltaSpeedX = -10.0f * GetSpeedModifier();
			DeltaSpeedY = 0.0f;

			MovementY = FVector(0.0f, DeltaSpeedX, 0.0f);

			CameraOwner->AddActorLocalOffset(MovementY, true);
			SetCameraDisable(true);
		}
		else if (ProportionY >= .975) { //TOP
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = 10.0f * GetSpeedModifier();

			MovementX = FVector(DeltaSpeedY, 0.0f, 0.0f);

			CameraOwner->AddActorLocalOffset(MovementX, true);
			SetCameraDisable(true);
		}
		else if (ProportionY <= .025) { //BOTTOM
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = -10.0f * GetSpeedModifier();

			MovementX = FVector(DeltaSpeedY, 0.0f, 0.0f);

			CameraOwner->AddActorLocalOffset(MovementX, true);
			SetCameraDisable(true);
		}
	}
		
}

//Zoom In
void UCameraMovementComponent::ZoomIn() {
	if (CameraOwner->GetCurrentArmLength() != MaxZoom) {
		CameraOwner->SetArmLength(-1.0f * DeltaArm);
		if (CameraOwner->GetCurrentArmLength() <= DefaultZoomLength) {
			CameraOwner->SetArmRotation(DeltaRotation);
		}
	}
}

//Zoom Out
void UCameraMovementComponent::ZoomOut() {
	if (CameraOwner->GetCurrentArmLength() != MinZoom) {
		CameraOwner->SetArmLength(DeltaArm);
		if (CameraOwner->GetCurrentArmLength() <= DefaultZoomLength) {
			CameraOwner->SetArmRotation(-1.0f * DeltaRotation);
		}
	}
}

//Default Zoom
void UCameraMovementComponent::DefaultZoom() {
	CameraOwner->SetToDefaultZoom();
}
