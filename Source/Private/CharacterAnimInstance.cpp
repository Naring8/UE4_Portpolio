#include "CharacterAnimInstance.h"


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
		bIsJumping = PawnOwner->GetMovementComponent()->IsFalling();
		bIsCrouching = PawnOwner->GetMovementComponent()->IsCrouching();
	}
}
