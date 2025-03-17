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
void UAssassinationComponent::Assassinate()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Assassinate!"));

	if (DataMap.Contains(EAssassinationType::Assassin) && DataMap.Contains(EAssassinationType::Assassinated))
	{
		FAssassinateData const* Actor = DataMap.Find(EAssassinationType::Assassin);
		FAssassinateData const* Actee = DataMap.Find(EAssassinationType::Assassinated);

		if (HitResult.bBlockingHit && HitResult.GetActor() != nullptr)
		{
			// typedef UStaticMeshComponent MeshType;	// typedef 도 지역의 영향을 받는다

			if (auto* const Owner = Cast<IAnimationInterface>(GetOwner()))
				Owner->PlayAnimation(Actor->Montage, Actor->PlayRate, Actor->Section);

			if (auto* const Victum = Cast<IAnimationInterface>(HitResult.GetActor()))
				Victum->PlayAnimation(Actee->Montage, Actee->PlayRate, Actee->Section);
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
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	))
		return true;
	else
		return false;
#pragma endregion
}
