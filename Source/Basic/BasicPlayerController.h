#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <../Interfaces/ControllerInterface.h>
#include <../Interfaces/CharacterMovementInterface.h>
#include "BasicPlayerController.generated.h"

UCLASS()
class UE4_PORTPOLIO_API ABasicPlayerController : public APlayerController, 
												 public IControllerInterface,
												 public ICharacterMovementInterface										
{
	GENERATED_BODY()
	
public:
	void SetupInputComponent() override;

private:
	void MoveFB(const float Value);
	void MoveLR(const float Value);
	void LookUD(const float Value) { AddPitchInput(Value); }
	void LookLR(const float Value) { AddYawInput(Value); }

	virtual void Walk() override;
	virtual void Run() override;

	virtual void ChangeWeapon() override;
	virtual void BaseAttack() override;
	virtual void Stealth() override;

	virtual void IgnoreInput(bool const LookInput, bool const MoveInput) override;

	virtual void SwitchCamera(class AActor* const ViewTarget, const float DelayTime = 1.0f) override;
};
