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
	}
}

void UAssassinationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

#include "../Interfaces/AnimationInterface.h"
#include "../Interfaces/CharacterStateInterface.h"
#include "../Interfaces/ControllerInterface.h"
#include "Kismet/GameplayStatics.h"
void UAssassinationComponent::Assassinate()
{
	if (DataMap.Contains(EAssassinationType::Assassin) && DataMap.Contains(EAssassinationType::Assassinated))
	{
		FAssassinateData const* Player = DataMap.Find(EAssassinationType::Assassin);
		FAssassinateData const* Enemy = DataMap.Find(EAssassinationType::Assassinated);

		if (HitResult.bBlockingHit && HitResult.GetActor() != nullptr)
		{
			if (auto const& PlayerController = Cast<IControllerInterface>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
				PlayerController->SwitchCamera(HitResult.GetActor(), 0.5f);

			// Player Code
			if (auto const& Owner = Cast<IAnimationInterface>(GetOwner()))
				Owner->PlayAnimation(Player->Montage, Player->PlayRate, Player->Section);

			// Enemy Code
			if (auto const& VictumState = Cast<ICharacterStateInterface>(HitResult.GetActor()))
				VictumState->SetCharacterState(ECharacterState::DEAD);
			if (auto const& Victum = Cast<IAnimationInterface>(HitResult.GetActor()))
				Victum->PlayAnimation(Enemy->Montage, Enemy->PlayRate, Enemy->Section);
		}
	}
}

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
bool UAssassinationComponent::TraceForward()
{
#pragma region TraceArrow
	FVector const Start = ForwardArrow->GetComponentLocation() + GetOwner()->GetActorLocation();
	FVector const End = Start + 
		(TraceDistance * 
			GetOwner()->GetActorForwardVector());

	if (UKismetSystemLibrary::LineTraceSingle
	(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(AssassinableCharacter), // Killable Object TraceChannel Collision
		false,
		{ GetOwner() },
		EDrawDebugTrace::None,
		HitResult,
		true
	))
	{
		if (auto const& VictumState = Cast<ICharacterStateInterface>(HitResult.GetActor()))
			if (VictumState->GetCharacterState() == ECharacterState::DEAD)
				return false;

		return true;
	}
	else
		return false;
#pragma endregion
}
