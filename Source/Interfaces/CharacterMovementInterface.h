#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <Animation/AnimInstance.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "CharacterMovementInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterMovementInterface : public UInterface
{
	GENERATED_BODY()
};

class UE4_PORTPOLIO_API ICharacterMovementInterface
{
	GENERATED_BODY()

public:
	virtual void LeftFoot() PURE_VIRTUAL(ICharacterMovementInterface::LeftFoot)
	virtual void RightFoot() PURE_VIRTUAL(ICharacterMovementInterface::RightFoot)

	virtual void Walk() PURE_VIRTUAL(ICharacterMovementInterface::Walk)
	virtual void Run() PURE_VIRTUAL(ICharacterMovementInterface::Run)

	virtual void CustomJump(bool IsJump) PURE_VIRTUAL(ICharacterMovementInterface::CustomJump)

	virtual void WallRunUp(bool WallUp) PURE_VIRTUAL(ICharacterMovementInterface::WallRunUp)

	virtual void ChangeWeapon() PURE_VIRTUAL(ICharacterMovementInterface::ChangeWeapon)
	virtual void BaseAttack() PURE_VIRTUAL(ICharacterMovementInterface::BaseAttack)
	virtual void Stealth() PURE_VIRTUAL(ICharacterMovementInterface::Stealth)
};
