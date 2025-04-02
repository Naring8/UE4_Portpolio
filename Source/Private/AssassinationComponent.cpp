#include <AssassinationComponent.h>

#include "../Basic/UE4_Portpolio.h"
#include <Engine/DataTable.h>
#include <GameFramework/Character.h>
#include <Components/ArrowComponent.h>

UAssassinationComponent::UAssassinationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterForward"));
}

void UAssassinationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AssassinateDataTable))
	{
		TArray<FAssassinateData const*> Rows;

		AssassinateDataTable->GetAllRows("", Rows);

		if (Rows.Num() > 0)
		{
			for (int32 Key = 0; Key < int32(EAssassinationType::Max); Key++)
			{
				FAssassinateData Value;

				for (auto& Row : Rows)
					if (Key == Row->Type)
						Value = *Row;

				DataMap.Add(EAssassinationType(Key), Value);
			}
		}

		Player = DataMap.Find(EAssassinationType::Assassin);
		Enemy = DataMap.Find(EAssassinationType::Assassinated);
	}
}

void UAssassinationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsValid(prevActor))
		SetCharacterWidget(prevActor, false);
}

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
bool UAssassinationComponent::TraceForward(AActor* const BaseActor)
{
	if (bCanTrace == false) return false;

#pragma region TraceArrow
	FVector const Start = ForwardArrow->GetComponentLocation() + BaseActor->GetActorLocation();
	FVector const End = Start +
		(TraceDistance *
			BaseActor->GetActorForwardVector());

	if (UKismetSystemLibrary::LineTraceSingle
	(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(AssassinableCharacter), // Killable Object TraceChannel Collision (Editor)
		false,
		{ BaseActor },
		EDrawDebugTrace::None,
		HitResult,
		true
	))
	{
		SetCharacterWidget(HitResult.GetActor(), true);
		prevActor = HitResult.GetActor();
		return true;
	}
#pragma endregion
	prevActor = nullptr;

	return false;
}

#include "../Interfaces/AnimationInterface.h"
#include "../Interfaces/CharacterStateInterface.h"
#include "../Interfaces/ControllerInterface.h"
#include "CharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"
void UAssassinationComponent::Assassinate()
{
	if (DataMap.Contains(EAssassinationType::Assassin) && DataMap.Contains(EAssassinationType::Assassinated))
	{
		if (HitResult.bBlockingHit && HitResult.GetActor() != nullptr)
		{
			bCanTrace = false; // Stop Trace while Animation Ended
			SetCharacterWidget(HitResult.GetActor(), false);

			if (auto const& PlayerController = Cast<IControllerInterface>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
				PlayerController->SwitchCamera(HitResult.GetActor(), CameraDelayTime);

			OwnerActor->SetActorLocation(HitResult.GetActor()->GetActorLocation() - (HitResult.GetActor()->GetActorForwardVector() * TraceDistance));
			OwnerActor->SetActorRotation(HitResult.GetActor()->GetActorRotation());

#pragma region PlayerAnimation
			if (auto const& OwnerCharacter = Cast<ACharacter>(OwnerActor))
				if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
					OwnerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::SetIdle);

			if (auto const& Owner = Cast<IAnimationInterface>(OwnerActor))
				Owner->PlayAnimation(Player->Montage, Player->PlayRate, Player->Section);
#pragma endregion


#pragma region EnemyAnimation
			if (auto const& EnemyCharacter = Cast<ACharacter>(HitResult.GetActor()))
				if (auto const& EnemyAnimInstance = Cast<UCharacterAnimInstance>(EnemyCharacter->GetMesh()->GetAnimInstance()))
					EnemyAnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::DeadAfterAnimation);
			if (auto const& Victum = Cast<IAnimationInterface>(HitResult.GetActor()))
				Victum->PlayAnimation(Enemy->Montage, Enemy->PlayRate, Enemy->Section);
#pragma endregion
		}
	}
}

void UAssassinationComponent::SetCharacterWidget(AActor* const Actor, bool const Detect)
{
	if (auto const& Character = Cast<ICharacterStateInterface>(Actor))
		Character->CharacterDetected(Detect);
}

void UAssassinationComponent::SetIdle(UAnimMontage* const Montage, bool const bInterrupted)
{
	bCanTrace = true;

	if (auto const& PlayerController = Cast<IControllerInterface>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		PlayerController->SwitchCamera(OwnerActor, 0.5f);
}

void UAssassinationComponent::DeadAfterAnimation(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (auto const& EnemyCharacter = Cast<ICharacterStateInterface>(HitResult.GetActor()))
		EnemyCharacter->CharacterDead();
}