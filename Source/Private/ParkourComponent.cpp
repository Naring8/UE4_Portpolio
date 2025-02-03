#include "ParkourComponent.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Animation/AnimInstance.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UpTrace = { 0.0f, 0.0f, 600.0f };
}

void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorsToIgnore.Add(Owner); // Owner를 무시하도록 추가

}


void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParkourComponent::BeginParkour(const float CapsuleHalfHeight, const float CapsuleRadius)
{
	if (IsValid(Owner)) // Is Owner Valid
	{
		OwnerCapsuleHalfHeight = CapsuleHalfHeight;
		OwnerCapsuleRadius = CapsuleRadius;

		TraceForward();
	}
}

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
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
	else
		Jumping();
}

void UParkourComponent::CheckObstacleHeight(FHitResult HitResult)
{
	SideLocation = HitResult.Location;
	SideNormal = HitResult.Normal;
	Rotation = UKismetMathLibrary::DegAcos(
		UKismetMathLibrary::Dot_VectorVector(
			UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector().GetSafeNormal(0.0001),
			Owner->GetActorForwardVector().GetSafeNormal(0.0001)));

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
		(Owner->GetActorLocation().Z - OwnerCapsuleHalfHeight);

	// Print On Screen
	/*GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow,
		FString::Printf(TEXT("Height: %f"), Height));*/

	FVector End = SideLocation - (UKismetMathLibrary::MakeRotFromX(SideNormal).Quaternion().GetForwardVector() * (60.0f));
	FVector Start = UpTrace + End;

	FHitResult temp;
	if (UKismetSystemLibrary::LineTraceSingle(
		this, Start, End,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1
		false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		temp,
		true, FLinearColor::Red
	))
		WallClimbingTest();
	else
		Vaulting();
}

void UParkourComponent::WallClimbingTest()
{
	FVector AddHalfHeight = FVector(SidePlace.X, SidePlace.Y, SidePlace.Z + OwnerCapsuleHalfHeight);
	const float QuarterHeight = OwnerCapsuleHalfHeight * 0.5f;

	FVector Start = FVector(AddHalfHeight.X, AddHalfHeight.Y, AddHalfHeight.Z + QuarterHeight);
	FVector End = FVector(AddHalfHeight.X, AddHalfHeight.Y, AddHalfHeight.Z - QuarterHeight);

	FHitResult Temp;
	if (UKismetSystemLibrary::SphereTraceSingle(
		this, Start, End,
		OwnerCapsuleRadius,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1
		false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Temp,
		true, FLinearColor::White
	))
		HasSide = false;
	else
		Jumping();
}

void UParkourComponent::Jumping()
{
	if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
		OwnerCharacter->Jump();
}


void UParkourComponent::Vaulting()
{
	if (IsValid(ParkourDataTable))
	{
		if (FindData())
			PlayMontage();
		else
			WallClimbingTest();
	}

}

bool UParkourComponent::FindData()
{
	if (IsValid(ParkourDataTable))
	{
		TArray<FParkourData const*> PDataArray;
		ParkourDataTable->GetAllRows("", PDataArray);

		for (auto const& PData : PDataArray)
		{
			if (UKismetMathLibrary::InRange_FloatFloat(Height, PData->DistMin, PData->DistMax, true, false))
			{
				PlayableParkourData = PData;
				return true;
			}
		}
	}

	return false;
}

#include <CharacterAnimInstance.h>
#include <Components/CapsuleComponent.h>
void UParkourComponent::PlayMontage()
{
	if (PlayableParkourData)
	{
		if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
		{
			IgnoreInput(true, false);
			if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
				OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::ResetValues);

			OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OwnerCharacter->PlayAnimMontage(PlayableParkourData->Montage, PlayableParkourData->PlayRate, PlayableParkourData->Section);
		}
	}
}

#include <../Basic/BasicPlayerController.h>
void UParkourComponent::ResetValues(UAnimMontage* Montage, bool bInterrupted)
{
	if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
	{
		PlayableParkourData = nullptr;
		OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			if (OwnerAnimInstance && OwnerAnimInstance->OnMontageEnded.IsBound())
				OwnerAnimInstance->OnMontageEnded.RemoveDynamic(this, &UParkourComponent::ResetValues);

		if (auto const& OwnerController = Cast<ABasicPlayerController>(OwnerCharacter->GetController()))
			OwnerController->ResetIgnoreInputFlags();
	}
}

void UParkourComponent::IgnoreInput(bool LookInput, bool MoveInput)
{
	if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
	{
		if (auto const& OwnerController = Cast<ABasicPlayerController>(OwnerCharacter->GetController()))
		{
			OwnerController->SetIgnoreLookInput(LookInput);
			OwnerController->SetIgnoreMoveInput(MoveInput);
		}
	}
}



