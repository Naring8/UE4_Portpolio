#include "BaseCharacter.h"
#include <Components/CapsuleComponent.h>
//#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

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

	/*if (GetMesh()->GetAnimInstance()->OnMontageEnded.IsBound())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("OnMontageEnded is still bound"));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("OnMontageEnded has been unboundd"));
	}*/
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#include <CharacterAnimInstance.h>
void ABaseCharacter::PlayAnimation(UAnimMontage* const Montage, float const PlayRate, FName const Section)
{
	if (!Montage)
		return;

	GetCharacterMovement()->StopMovementImmediately();
	GetController()->SetIgnoreLookInput(true);
	GetController()->SetIgnoreMoveInput(true);

	PlayAnimMontage(Montage, PlayRate, Section);

	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::ResetToIdle);
}

#include <../Basic/BasicPlayerController.h>
void ABaseCharacter::ResetToIdle(UAnimMontage* const Montage, bool const bInterrupted)
{
	// TODO: IT BIND IN OnMontageEnded but this Function is not calling
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, TEXT("Montage Ended Successfully"));

	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
		if (OwnerAnimInstance->OnMontageEnded.IsBound())
			OwnerAnimInstance->OnMontageEnded.RemoveDynamic(this, &ThisClass::ResetToIdle);

	GetController()->ResetIgnoreInputFlags();
}

void ABaseCharacter::CustomCrouch()
{
	Crouch();
}

void ABaseCharacter::CustomUncrouch()
{
	UnCrouch();
}

void ABaseCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void ABaseCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}