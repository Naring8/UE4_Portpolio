#include "ParkourComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorsToIgnore.Add(Owner.Get()); // Owner를 무시하도록 추가
}


void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParkourComponent::BeginParkour()
{
	if (Owner.IsValid())
		TraceForward();
}

// Owner는 BeginParkour에서 이미 검사됨
void UParkourComponent::TraceForward()
{
	FHitResult HitResult;
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * 150.0f;

	// TODO : Check TraceChannel
	if (UKismetSystemLibrary::CapsuleTraceSingle
	(
		this, Start, End,
		5.0f, 90.0f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true, FLinearColor::Black
	))
		CheckObstacleHeight(HitResult);
}

void UParkourComponent::CheckObstacleHeight(FHitResult HitResult)
{
	const FVector SideLocation = HitResult.Location;
	const FVector SideNormal = HitResult.Normal;
	const float Rotation = UKismetMathLibrary::DegAcos(
		UKismetMathLibrary::Dot_VectorVector(
			UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector().GetSafeNormal(0.0001),
			Owner.Get()->GetActorForwardVector().GetSafeNormal(0.0001)));

	FVector UpTrace = { 0.0f, 0.0f, 600.0f };

	FVector End = SideLocation - UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector() * 40.0f;
	FVector Start = UpTrace + End;

	FHitResult ObstacleHitResult;
	UKismetSystemLibrary::CapsuleTraceSingle
	(
		this, Start, End,
		5.0f, 90.0f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		ObstacleHitResult,
		true, FLinearColor::Blue
	);
}

void UParkourComponent::CheckObstacleThickness()
{
}

