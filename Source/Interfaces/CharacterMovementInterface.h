#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
};
