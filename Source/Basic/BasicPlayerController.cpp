#include "BasicPlayerController.h"

void ABasicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveFB", this, &ThisClass::MoveFB);
	InputComponent->BindAxis("MoveLR", this, &ThisClass::MoveLR);
	InputComponent->BindAxis("LookUD", this, &ThisClass::LookUD);
	InputComponent->BindAxis("LookLR", this, &ThisClass::LookLR);

	InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ThisClass::Walk);
	InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ThisClass::Run);

	InputComponent->BindAction("Weapons", EInputEvent::IE_Pressed, this, &ThisClass::ChangeWeapon);
	InputComponent->BindAction("BaseAttack", EInputEvent::IE_Pressed, this, &ThisClass::BaseAttack);
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

void ABasicPlayerController::Walk()
{
	if (auto* const ControlledPawn = Cast<ICharacterMovementInterface>(GetPawn()))
		ControlledPawn->Walk();
}

void ABasicPlayerController::Run()
{
	if (auto* const ControlledPawn = Cast<ICharacterMovementInterface>(GetPawn()))
		ControlledPawn->Run();
}

void ABasicPlayerController::ChangeWeapon()
{
	if (auto* const ControlledPawn = Cast<ICharacterMovementInterface>(GetPawn()))
		ControlledPawn->ChangeWeapon();
}

void ABasicPlayerController::BaseAttack()
{
	if (auto* const ControlledPawn = Cast<ICharacterMovementInterface>(GetPawn()))
		ControlledPawn->BaseAttack();
}

void ABasicPlayerController::IgnoreInput(bool const LookInput, bool const MoveInput)
{
	SetIgnoreLookInput(LookInput);
	SetIgnoreMoveInput(MoveInput);
}

void ABasicPlayerController::SwitchCamera(AActor* const ViewTarget, const float DelayTime)
{
	SetViewTargetWithBlend(ViewTarget, DelayTime);
}
