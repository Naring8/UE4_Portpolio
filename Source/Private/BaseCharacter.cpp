#include "BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �̵� �������� ȸ��
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set Walk
	Walk(); // Default

	// Set Crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Jump Scale
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// SpringArmComponent ���� �� ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true; // ī�޶� ��Ʈ�ѷ� ȸ���� �������� ����
	SpringArm->bDoCollisionTest = true; // �浹 �׽�Ʈ Ȱ��ȭ
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// CameraComponent ���� �� ����
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // ī�޶� ĳ������ ȸ���� �����ϵ��� ����

	// (Capsule, Forward Vector)�� �µ��� Mesh ��ġ, ȸ�� ����
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());

	// NEEDS: Change Capulse Size depends on CharacterState
	GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight);
	GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius);
}

void ABaseCharacter::OnConstruction(FTransform const& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterState == ECharacterState::DEAD)
		return;
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#include "CharacterAnimInstance.h"
#include "../Interfaces/ControllerInterface.h"
void ABaseCharacter::PlayAnimation(UAnimMontage* const Montage, float const PlayRate, FName const Section)
{
	if (!Montage)
		return;

	GetCharacterMovement()->StopMovementImmediately();
	if (auto const& PlayerController = Cast<IControllerInterface>(GetController()))
		PlayerController->IgnoreInput(true, true);

	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
		OwnerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::ResetToIdle);

	PlayAnimMontage(Montage, PlayRate, Section);
}

void ABaseCharacter::ResetToIdle(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
		OwnerAnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::ResetToIdle);

	if (CharacterState == ECharacterState::DEAD)
	{
		Die();
		return;
	}

	if (auto const& PlayerController = Cast<IControllerInterface>(GetController()))
		PlayerController->SwitchCamera(this);

	GetController()->ResetIgnoreInputFlags();

	if (GetCharacterMovement()->IsCrouching())
		GetCharacterMovement()->UnCrouch();
}

void ABaseCharacter::Die()
{
	//if (CharacterState == ECharacterState::DEAD) return; // �̹� ����� ���¶�� �ߺ� ���� ����

	//CharacterState = ECharacterState::DEAD;

	// Stop Animation
	GetMesh()->bPauseAnims = true;

	// Disable Movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	// Disable Input
	if (auto const& PlayerController = Cast<APlayerController>(GetController()))
		PlayerController->DisableInput(PlayerController);
}
