#include "BasicPlayerController.h"

void ABasicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveFB", this, &ThisClass::MoveFB);
	InputComponent->BindAxis("MoveLR", this, &ThisClass::MoveLR);
	InputComponent->BindAxis("LookUD", this, &ThisClass::LookUD);
	InputComponent->BindAxis("LookLR", this, &ThisClass::LookLR);
}

// BindAxis Movement
void ABasicPlayerController::MoveFB(const float Value)
{
	if (auto* const ControlledPawn = GetPawn())
	{
		FVector const Direction = FRotator(0, GetControlRotation().Yaw, 0).Quaternion().GetForwardVector();

		ControlledPawn->AddMovementInput(Direction, Value);
	}
}

void ABasicPlayerController::MoveLR(const float Value)
{
	if (auto* const ControlledPawn = GetPawn())
	{
		FVector const Direction = FRotator(0, GetControlRotation().Yaw, 0).Quaternion().GetRightVector();

		ControlledPawn->AddMovementInput(Direction, Value);
	}
}

void ABasicPlayerController::LookUD(const float Value)
{
	AddPitchInput(Value);
}

void ABasicPlayerController::LookLR(const float Value)
{
	AddYawInput(Value);
}
