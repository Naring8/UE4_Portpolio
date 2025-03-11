#include "AssassinationComponent.h"
#include <../Basic/UE4_Portpolio.h>
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

}


void UAssassinationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (auto const& OwnerCharacter = Cast<ACharacter>(GetOwner()))
		TraceForward();
}

void UAssassinationComponent::Assassinate()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Assassinate!"));
}

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
void UAssassinationComponent::TraceForward()
{
	AActor* HitObstacle = nullptr;
	FVector HitObstacleExtent = FVector::ZeroVector;
	float HitDistance = 0;
	float YawToFace = 0;

	FVector WallLocation = FVector::ZeroVector;
	FVector WallNormal = FVector::ZeroVector;

#pragma region TraceArrow
	FVector const Start = ForwardArrow->GetComponentLocation() + GetOwner()->GetActorLocation();
	FVector const End = Start + 
		(TraceDistance * 
			GetOwner()->GetActorForwardVector());

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(AssassinableCharacter), // Can Climb TraceChannel Collision
		false,
		{ GetOwner() },
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);
#pragma endregion

	if (HitResult.bBlockingHit && HitResult.GetActor() != nullptr)
	{
		typedef UStaticMeshComponent MeshType;	// typedef 도 지역의 영향을 받는다

		if (MeshType const* const Mesh = Cast<MeshType>(HitResult.Actor->GetComponentByClass(MeshType::StaticClass())))
		{
			HitObstacle = HitResult.GetActor();

			FVector Min, Max;

			Mesh->GetLocalBounds(Min, Max);

			HitObstacleExtent = (Max - Min) * HitObstacle->GetActorScale();
			HitDistance = HitResult.Distance;
			YawToFace = UKismetMathLibrary::MakeRotFromX(-HitResult.Normal).Yaw;

			WallLocation = HitResult.Location;
			WallNormal = HitResult.Normal;
		}
	}
}
