#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Operator.h"

#include "CameraMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_V1_API UCameraMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Get Attached Pawn
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
		AOperator* CameraOwner;

	// Character Controller Reference
	UPROPERTY(BlueprintReadWrite)
		APlayerController* PlayerControllerRef;

protected:
	// Attributes

	// Bools

	// Is Movement Disabled?
	UPROPERTY(Category = "Movement Var: Bools", BlueprintReadOnly)
		bool bDisableCameraMovement;

	// Movement Speeds
	UPROPERTY(Category = "Movement Speeds", BlueprintReadOnly)
		float DefaultMovementSpeed;

	// Modify Default Movement Speed
	UPROPERTY(Category = "Movement Speeds", BlueprintReadOnly)
		float MovementSpeedModifier;

	// Camera Direction
	// Gets Forward DIrection of Camera Pawn
	UPROPERTY(Category = "Movement Speeds", BlueprintReadOnly)
		FVector FaceDirection;

	// Zoom
	// Default
	UPROPERTY(Category = "Zoom", BlueprintReadOnly)
		float DefaultZoomLength;

	// Amount Camera Arm Changes
	UPROPERTY(Category = "Zoom", BlueprintReadOnly)
		float DeltaArm;

	// Amount Rotation Changes
	UPROPERTY(Category = "Zoom", BlueprintReadOnly)
		FRotator DeltaRotation;

	// Farthest Camera Can Get on ZoomOut
	UPROPERTY(Category = "Zoom", BlueprintReadOnly)
		float MaxZoom;

	//Closest Arm Can ZoomIn
	UPROPERTY(Category = "Zoom", BlueprintReadOnly)
		float MinZoom;

public:
	//Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Speed")
		FORCEINLINE float GetCurrentMovementSpeed() { return DefaultMovementSpeed; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Speed")
		FORCEINLINE float GetSpeedModifier() { return MovementSpeedModifier; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Speed")
		FORCEINLINE bool IsCameraDisable() { return bDisableCameraMovement; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Location")
		FVector GetCameraLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Direction")
		FVector GetFaceDirection();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Direction")
		FRotator GetCameraRotation();

	//Setters
	UFUNCTION(BlueprintCallable, Category = "Camera Speed")
		float SetMovementSpeedModifier(float ModifierAmount);

	UFUNCTION(BlueprintCallable, Category = "Camera Speed")
		float SetMovementSpeed(float MovementSpeedAdjustment);

	UFUNCTION(BlueprintCallable, Category = "Camera Speed")
		bool SetCameraDisable(bool bDisableCamera);

	//Utility
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		virtual void BasicMovementControl(float AxisValueX, float AxisValueY);

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		virtual void PanCamera(float RotationAmount);

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		virtual void ResetPan();

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		virtual void EdgeScroll();

	UFUNCTION(BlueprintCallable, Category = "Camera Zoom")
		virtual void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Camera Zoom")
		virtual void ZoomOut();

	UFUNCTION(BlueprintCallable, Category = "Camera Zoom")
		virtual void DefaultZoom();

};
