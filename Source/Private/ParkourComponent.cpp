#include "ParkourComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParkourComponent::BeginParkour()
{
	TraceForward();
}

void UParkourComponent::TraceForward()
{
	if (auto* const Owner = GetOwner())
	{
		FHitResult HitResult;
		FVector Start = Owner->GetActorLocation();
		FVector End = Start + Owner->GetActorForwardVector() * 150.0f;

		// TODO : Check TraceChannel
		UKismetSystemLibrary::CapsuleTraceSingle
		(
			this, Start, End,
			5.0f, 90.0f,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1,
			false,
			{ Owner },
			EDrawDebugTrace::Persistent,
			HitResult,
			true, FLinearColor::Black
		);
	}
}

