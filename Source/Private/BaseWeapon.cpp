#include "BaseWeapon.h"

#include <Components/CapsuleComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>

#include <MontageDataAsset.h>
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(Capsule);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMesh->SetupAttachment(Capsule);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//if mesh get collision, this make Owner's Movement odd

	Assassination = CreateDefaultSubobject<UAssassinationComponent>("Assassination");
}

#include "CharacterAnimInstance.h"
#include "GameFramework/Character.h"
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	DisableAttack();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Assassination->SetOwnerActor(GetOwner());

	if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
	{
		OwnerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::AttackContinued);
		OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::AttackEnded);
	}
}

#include <../Interfaces/CharacterStateInterface.h>
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAttackEnabled == false) return;

	if(auto const& OwnerState = Cast<ICharacterStateInterface>(OwnerCharacter))
		if((OwnerCharacter->bIsCrouched) && (OwnerState->GetCharacterState() == ECharacterState::IDLE))
			if (Assassination->TraceForward(OwnerCharacter))
				bCanAssassinate = true;
}

void ABaseWeapon::BaseAttack()
{
	if (bAttackEnabled == false) return; // Do not carrying weapon

	if (bAttackTransisted) return; // if more than second input while ACTING

	if (auto const& OwnerState = Cast<ICharacterStateInterface>(GetOwner()))
	{
		if (OwnerState->GetCharacterState() == ECharacterState::ACTING) // combo
			bAttackTransisted = true;
		else // First Attack
			OwnerState->SetCharacterState(ECharacterState::ACTING);
	}

	if (!bAttackTransisted)
		PlayWeaponAssetMontage();
}

void ABaseWeapon::DoAssassinate()
{
	if (bCanAssassinate)
		Assassination->Assassinate();
}

#include <../Interfaces/ControllerInterface.h>
void ABaseWeapon::PlayWeaponAssetMontage()
{
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCharacterMovement()->StopMovementKeepPathing();

		if (auto const& OwnerController = Cast<IControllerInterface>(OwnerCharacter->GetController()))
			OwnerController->IgnoreInput(false, true);

		FMontageData const CurMontage = ActingData->GetMontageData(AttackIndex);

		OwnerCharacter->PlayAnimMontage(CurMontage.Montage, CurMontage.PlayRate, CurMontage.Section);
	}
}

void ABaseWeapon::EnableAttack()
{
	if (IsValid(StaticMesh))
		StaticMesh->SetVisibility(true);
	Capsule->SetGenerateOverlapEvents(true);

	bAttackEnabled = true;
}

void ABaseWeapon::DisableAttack()
{
	if (IsValid(StaticMesh)) // "Bare Hands" do not get mesh
		StaticMesh->SetVisibility(false);
	Capsule->SetGenerateOverlapEvents(false);

	bAttackEnabled = false;
}

#include <../Interfaces/AnimationInterface.h>
void ABaseWeapon::ActivateAttack()
{
}

void ABaseWeapon::DeactivateAttack()
{
}

void ABaseWeapon::AttackContinued(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (bAttackTransisted)
	{
		bAttackTransisted = false; ++AttackIndex;
		PlayWeaponAssetMontage();
	}
}

void ABaseWeapon::AttackEnded(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (!bInterrupted) // if Montage Ended Successfully (not blend out)
	{
		if (auto const& OwnerState = Cast<ICharacterStateInterface>(GetOwner()))
			OwnerState->SetCharacterState(ECharacterState::IDLE);

		OwnerCharacter->GetController()->ResetIgnoreInputFlags();

		AttackIndex = 0;
		bAttackTransisted = false;
	}

	/*if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
	{
		if (OwnerAnimInstance->OnMontageBlendingOut.IsBound())
			OwnerAnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::AttackContinued);

		if (OwnerAnimInstance->OnMontageEnded.IsBound())
			OwnerAnimInstance->OnMontageEnded.RemoveDynamic(this, &ThisClass::AttackEnded);
	}*/
}

//#include "Blueprint/UserWidget.h"
//void ABaseWeapon::SetWidgetVisibility(ESlateVisibility const Visibility)
//{
//	PickUpWidget->SetVisibility(Visibility);
//}

