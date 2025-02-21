#pragma once

#include "CoreMinimal.h"
#include <Animation/AnimInstance.h>
#include <../Interfaces/CharacterMovementInterface.h>
#include "CharacterAnimInstance.generated.h"

UCLASS()
class UE4_PORTPOLIO_API UCharacterAnimInstance : public UAnimInstance,
												 public ICharacterMovementInterface
{
	GENERATED_BODY()

public:
	void NativeUpdateAnimation(float const DeltaSeconds) override;

private:
	virtual void WallRunUp(bool WallUp) override { bWallRunUp = WallUp; }
	virtual void CustomJump(bool IsJump) override { bIsJump = IsJump; }

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Direction;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Speed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Yaw;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Pitch;


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		bool bIsJump = false;
private:
#pragma region ParkourComponent
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		bool bWallRunUp = false;
#pragma endregion
	
};
