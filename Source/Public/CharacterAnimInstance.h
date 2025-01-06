#pragma once

#include "CoreMinimal.h"
#include <Animation/AnimInstance.h>
#include "CharacterAnimInstance.generated.h"

UCLASS()
class UE4_PORTPOLIO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void NativeUpdateAnimation(float const DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Direction;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Speed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Yaw;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Pitch;
	
};
