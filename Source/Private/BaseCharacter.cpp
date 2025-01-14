#include "BaseCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �̵� �������� ȸ��
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// SpringArmComponent ���� �� ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true; // ī�޶� ��Ʈ�ѷ� ȸ���� �������� ����
	SpringArm->bDoCollisionTest = false; // �浹 �׽�Ʈ ��Ȱ��ȭ
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// CameraComponent ���� �� ����
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // ī�޶� ĳ������ ȸ���� �����ϵ��� ����

	// (Capsule, Forward Vector)�� �µ��� Mesh ��ġ, ȸ�� ����
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());

	// TODO: Change Capulse Size depends on CharacterState
	GetCapsuleComponent()->SetCapsuleHalfHeight(95.0f);
	GetCapsuleComponent()->SetCapsuleRadius(35.0f);
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

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

