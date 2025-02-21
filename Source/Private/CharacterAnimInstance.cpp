#include "CharacterAnimInstance.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

void UCharacterAnimInstance::NativeUpdateAnimation(float const DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (auto* const PawnOwner = TryGetPawnOwner())
	{
		FVector const Velocity = PawnOwner->GetVelocity();

		Direction = CalculateDirection(Velocity, PawnOwner->GetActorRotation());
		Speed = Velocity.Size2D();

		Pitch = PawnOwner->GetBaseAimRotation().Pitch;
		Yaw = PawnOwner->GetBaseAimRotation().Yaw;

		// Is In Air?
		bIsJump = PawnOwner->GetMovementComponent()->IsFalling();
	}
}
