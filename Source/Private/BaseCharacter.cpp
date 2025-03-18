#include "BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 이동 방향으로 회전
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

	// SpringArmComponent 생성 및 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true; // 카메라가 컨트롤러 회전을 따르도록 설정
	SpringArm->bDoCollisionTest = true; // 충돌 테스트 활성화
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// CameraComponent 생성 및 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // 카메라가 캐릭터의 회전을 무시하도록 설정

	// (Capsule, Forward Vector)에 맞도록 Mesh 위치, 회전 변경
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
	//if (CharacterState == ECharacterState::DEAD) return; // 이미 사망한 상태라면 중복 실행 방지

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
