#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Operator.h"

#include "RTSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_V1_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARTSPlayerController();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	//Call Child Methods
	virtual void CallMoveForward(float Value);
	virtual void CallMoveRight(float Value);

	virtual void CallIncreaseSpeedModifier();
	virtual void CallDecreaseSpeedModifier();

	virtual void CallPanLeft();
	virtual void CallPanRight();
	virtual void CallPanReset();

	virtual void CallZoomIn();
	virtual void CallZoomOut();
	virtual void CallZoomReset();

protected:
	//Overrides
	virtual void BeginPlay() override;

public:
	//Attributes
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
		AOperator* OperatorRef;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
		UCameraMovementComponent* CameraMovementRef;
};
