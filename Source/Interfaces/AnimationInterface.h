#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimationInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

class UE4_PORTPOLIO_API IAnimationInterface
{
	GENERATED_BODY()

public:
	virtual void PlayAnimation(class UAnimMontage* const Montage, float const PlayRate, FName const Section) PURE_VIRTUAL(IAnimationInterface::PlayAnimation)
	virtual void PlayContinuousAnimation(class UAnimMontage* const Montage, float const PlayRate, FName const Section) PURE_VIRTUAL(IAnimationInterface::PlayContinuousAnimation)
};