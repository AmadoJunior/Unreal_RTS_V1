#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include "CameraMovementComponent.h"
#include "Components/ActorComponent.h"

#include "Operator.generated.h"

class UCameraMovementComponent;

UCLASS()
class RTS_V1_API AOperator : public APawn
{
	GENERATED_BODY()

public:
	// Constructor
	AOperator();

	// Tick
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UPROPERTY(Category = "Movement Component", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))*/
	UCameraMovementComponent* PawnMovementComponent;

protected:
	// Collision Sphere Root Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Root Comp")
		USphereComponent* CollisionSphere;
	// Player Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* PlayerCamera;

	// Spring Arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* CameraArm;

	//Default CameraArm Length
	UPROPERTY(Category = "Movement Variables: Zoom", BlueprintReadOnly)
		float DefaultZoomLength;

	//Defaul Rotation
	UPROPERTY(Category = "Movement Variables: Zoom", BlueprintReadOnly)
		FRotator DefaultCameraRotation;

public:
	//Getters
	// Returns Camera
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		FORCEINLINE class UCameraComponent* GetCamera() const { return PlayerCamera; }

	// Returns Camera Arm
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		FORCEINLINE class USpringArmComponent* GetCameraArm() const { return CameraArm; }

	// Returns Length of Camera Arm
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Zoom")
		float GetCurrentArmLength();

	// Returns Camera Arm Rotation
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Zoom")
		FRotator GetCurrentRotation();

	// Setters
	UFUNCTION(BlueprintCallable, Category = "Camera Zoom")
		virtual void SetArmLength(float ChangeAmount);

	UFUNCTION(BlueprintCallable, Category = "Camera Zoom")
		virtual void SetArmRotation(FRotator ChangeAmount);

	UFUNCTION(BlueprintCallable, Category = "Camera Zoom")
		virtual void SetToDefaultZoom();
};
