#include "BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include <BaseWeapon.h>

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

#pragma region AttachWeapons
	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = SpawnParameters.Instigator = this;

	for (auto& WeaponClass : WeaponClasses)
	{
		if (!IsValid(WeaponClass))
			continue;

		auto* const SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParameters);

		FAttachmentTransformRules const Rules(EAttachmentRule::KeepRelative, true);

		SpawnedWeapon->AttachToComponent(GetMesh(), Rules, SpawnedWeapon->GetWeaponSocketName());

		Weapons.Add(SpawnedWeapon);
	}

	if (Weapons.Num() > 0)
	{
		if(auto const& Weapon = Cast<IWeaponInterface>(Weapons[WeaponIdx]))
			Weapon->EnableAttack();
	}
#pragma endregion

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
	if (!Montage) return;

	GetCharacterMovement()->StopMovementImmediately();
	if (auto const& PlayerController = Cast<IControllerInterface>(GetController()))
	{
		PlayerController->IgnoreInput(true, false);
		DisableInput(GetController()->CastToPlayerController());
	}

	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
		OwnerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::ResetToIdle);

	PlayAnimMontage(Montage, PlayRate, Section);
}

void ABaseCharacter::ResetToIdle(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
		if(OwnerAnimInstance->OnMontageBlendingOut.IsBound())
			OwnerAnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::ResetToIdle);

	CharacterState = ECharacterState::IDLE;

	EnableInput(GetController()->CastToPlayerController());
	GetController()->ResetIgnoreInputFlags();
}

void ABaseCharacter::CharacterDead()
{
	if (CharacterState == ECharacterState::DEAD) return;
	CharacterState = ECharacterState::DEAD;

	// Set Weapon All Disabled
	for (auto const& Weapon : Weapons)
		if(auto const& WeaponStatus = Cast<IWeaponInterface>(Weapon))
			WeaponStatus->DisableAttack();

	// Stop Animation
	GetMesh()->bPauseAnims = true;

	// Disable Movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable Input
	if (auto const& PlayerController = Cast<APlayerController>(GetController()))
		PlayerController->DisableInput(PlayerController);
}

void ABaseCharacter::ChangeWeapon()
{
	if (Weapons.Num() > 0)
	{
		if (auto const& PrevWeapon = Cast<IWeaponInterface>(Weapons[WeaponIdx]))
			PrevWeapon->DisableAttack();

		++WeaponIdx %= Weapons.Num();

		if (auto const& CurWeapon = Cast<IWeaponInterface>(Weapons[WeaponIdx]))
			CurWeapon->EnableAttack();
	}
}

void ABaseCharacter::BaseAttack()
{
	Weapons[WeaponIdx]->BaseAttack();
}
