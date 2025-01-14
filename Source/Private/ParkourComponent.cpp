#include "ParkourComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UpTrace = { 0.0f, 0.0f, 600.0f };
}

void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorsToIgnore.Add(GetOwner()); // Owner를 무시하도록 추가
}


void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParkourComponent::BeginParkour(const float CapsuleHalfHeight, const float CapsuleRadius)
{
	if (Owner.IsValid())
	{
		OwnerCapsuleHalfHeight = CapsuleHalfHeight;
		OwnerCapsuleRadius = CapsuleRadius;

		TraceForward();
	}
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
		5.0f, OwnerCapsuleHalfHeight,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1
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
	SideLocation = HitResult.Location;
	SideNormal = HitResult.Normal;
	Rotation = UKismetMathLibrary::DegAcos(
		UKismetMathLibrary::Dot_VectorVector(
			UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector().GetSafeNormal(0.0001),
			Owner.Get()->GetActorForwardVector().GetSafeNormal(0.0001)));

	// FVector UpTrace = { 0.0f, 0.0f, 600.0f };

	FVector End = SideLocation - UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector() * 40.0f;
	FVector Start = UpTrace + End;

	FHitResult ObstacleHitResult;
	if (UKismetSystemLibrary::CapsuleTraceSingle (
		this, Start, End,
		OwnerCapsuleRadius, OwnerCapsuleHalfHeight,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		ObstacleHitResult,
		true, FLinearColor::Blue
	))
		CheckObstacleThickness(ObstacleHitResult);
}

void UParkourComponent::CheckObstacleThickness(FHitResult HitResult)
{
	SidePlace = HitResult.Location;
	Height = HitResult.ImpactPoint.Z -
		(Owner.Get()->GetActorLocation().Z - OwnerCapsuleHalfHeight);

	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow,
		FString::Printf(TEXT("Height: %f"), Height));

	FVector End = SideLocation - (UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector() * (60.0f));
	FVector Start = UpTrace + End;

	FHitResult temp;
	if (UKismetSystemLibrary::LineTraceSingle(
		this, Start, End,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1
		false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		temp,
		true, FLinearColor::Red))
	{
		WallClimbingTest();
	}
	else
	{

	}
}

void UParkourComponent::WallClimbingTest()
{
	/*FVector Start = SidePlace + FVector({ 0.0f, 0.0f }, OwnerCapsuleHalfHeight);
	if(UKismetSystemLibrary::SphereTraceSingle (

	
	
	))*/
}

