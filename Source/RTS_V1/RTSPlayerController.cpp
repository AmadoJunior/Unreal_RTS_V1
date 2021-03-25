#include "RTSPlayerController.h"
#include "Components/InputComponent.h"
#include "CameraMovementComponent.h"

//Constructor
ARTSPlayerController::ARTSPlayerController() {
	//Defaults
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void ARTSPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//EdgeScroll
	CameraMovementRef->EdgeScroll();
}

void ARTSPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	//Set Up Gameplay KeyBindings
	check(InputComponent);

	//Axis
	InputComponent->BindAxis("MoveForward", this, &ARTSPlayerController::CallMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARTSPlayerController::CallMoveRight);
	//Actions
	//Increase Movement Speed While Shift Pressed
	InputComponent->BindAction("SpeedModifier", IE_Pressed, this, &ARTSPlayerController::CallIncreaseSpeedModifier);
	InputComponent->BindAction("SpeedModifier", IE_Released, this, &ARTSPlayerController::CallDecreaseSpeedModifier);
	//Pan
	InputComponent->BindAction("PanCameraLeft", IE_Pressed, this, &ARTSPlayerController::CallPanLeft);
	InputComponent->BindAction("PanCameraRight", IE_Pressed, this, &ARTSPlayerController::CallPanRight);
	InputComponent->BindAction("PanReset", IE_Pressed, this, &ARTSPlayerController::CallPanReset);
	//Zoom
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTSPlayerController::CallZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTSPlayerController::CallZoomOut);
	InputComponent->BindAction("ZoomReset", IE_Pressed, this, &ARTSPlayerController::CallZoomReset);
}

void ARTSPlayerController::BeginPlay() {
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("BeginPlay"));
	//Ref Casts
	OperatorRef = Cast<AOperator>(GetPawn()); //Gets Camera Pawn
	CameraMovementRef = OperatorRef->MovementComponent;

	//Show Mouse
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

//Methods
void ARTSPlayerController::CallMoveForward(float Value) {
	if (Value > 0 || Value < 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(Value));
	}
	
	CameraMovementRef->BasicMovementControl(Value, 0.0f);
}

void ARTSPlayerController::CallMoveRight(float Value) {
	if (Value > 0 || Value < 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(Value));
	}
	CameraMovementRef->BasicMovementControl(0.0f, Value);
}

void ARTSPlayerController::CallIncreaseSpeedModifier() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallIncreaseSpeedModifier"));
	CameraMovementRef->SetMovementSpeedModifier(2.0f);
}

void ARTSPlayerController::CallDecreaseSpeedModifier() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallDecreaseSpeedModifier"));
	CameraMovementRef->SetMovementSpeedModifier(1.0f);
}

void ARTSPlayerController::CallPanLeft() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallPanLeft"));
	CameraMovementRef->PanCamera(45.0f);
}

void ARTSPlayerController::CallPanRight() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallPanRight"));
	CameraMovementRef->PanCamera(-45.0f);
}

void ARTSPlayerController::CallPanReset() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallPanReset"));
	CameraMovementRef->ResetPan();
}

void ARTSPlayerController::CallZoomIn() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallZoomIn"));
	CameraMovementRef->ZoomIn();
}

void ARTSPlayerController::CallZoomOut() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallZoomOut"));
	CameraMovementRef->ZoomOut();
}

void ARTSPlayerController::CallZoomReset() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CallZoomReset"));
	CameraMovementRef->DefaultZoom();
}
