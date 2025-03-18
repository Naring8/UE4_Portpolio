#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class UE4_PORTPOLIO_API IControllerInterface
{
	GENERATED_BODY()

public:
	virtual void IgnoreInput(bool const LookInput, bool const MoveInput) PURE_VIRTUAL(IControllerInterface::IgnoreInput)
	virtual void SwitchCamera(class AActor* const ViewTarget, const float DelayTime = 1.0f) PURE_VIRTUAL(IControllerInterface::SwitchCamera)
};
