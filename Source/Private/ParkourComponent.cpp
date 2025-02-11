#include "ParkourComponent.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

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

	// Parkour Data Table => Data Map (STL)
	if (IsValid(ParkourDataTable))
	{
		TArray<FParkourData const*> Rows;

		ParkourDataTable->GetAllRows("", Rows);	// "" 안에 들어간 내용이 들어간 문자열만 받아옴 / "" << 아무것도 없기 때문에 모두 가져옴

		if (Rows.Num() > 0)
		{
			for (int32 Key = 0; Key < int32(EParkourType::Max); Key++)
			{
				TArray<FParkourData> Values;

				for (auto& Row : Rows)
					if (Key == Row->Type)
						Values.Add(*Row);

				DataMap.Add(EParkourType(Key), Values);
			}
		}
	}
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

#include <CharacterAnimInstance.h>
#include <Components/CapsuleComponent.h>
void UParkourComponent::BeginParkour()
{
	if (auto const& OwnerCharacter = Cast<ACharacter>(Owner)) // Is Owner Valid
	{
		OwnerCapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		OwnerCapsuleRadius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

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
	if (UKismetSystemLibrary::CapsuleTraceSingle(
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
		true, FLinearColor::Red
	))
		HasSide = false;
	else
		CheckWall();
}

void UParkourComponent::CheckWall()
{
	if ((Height > 40.0f) && (140.0f <= Rotation && Rotation <= 220.0f))
	{
		PlayableParkourData = FindData(EParkourType::Jump);
		if (PlayableParkourData)
		{
			float delayTime = ObstacleGap / 400.0f;
			if ((delayTime > 0.0f) && (Height > 200.0f))
			{
				FTimerHandle TimerHandle;

				//타이머 설정 (delayTime 초 후 한번 실행)
				GetWorld()->GetTimerManager().SetTimer
				(
					TimerHandle,                                // 핸들
					this,                                       // 대상 객체
					&ThisClass::PlayMontage,
					delayTime,                                  // 반복 주기(초)
					false,                                      // 반복 여부
					delayTime                                   // 초기 지연 시간
				);

				if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
				{
					OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

					FVector MoveLocation = (SidePlace - FVector(0.0f, 0.0f, 200.0f)) + (OwnerCharacter->GetActorForwardVector() * (-40.0f));
					//FVector MoveLocation = (SidePlace - FVector(0.0f, 0.0f, 200.0f));

					FLatentActionInfo LatentInfo; // Completed

					// TODO: Need More Implement (do not arrive at delayTime)
					UKismetSystemLibrary::MoveComponentTo(OwnerCharacter->GetCapsuleComponent(),
						MoveLocation,
						OwnerCharacter->GetActorRotation(),
						false,
						false,
						delayTime,
						false,
						EMoveComponentAction::Move,
						LatentInfo);

					OwnerCharacter->PlayAnimMontage(WallClimb, 1.0f);	
				}
			}
		}
		else
			BracedDrop();
	}
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
	PlayableParkourData = FindData(EParkourType::Vault);

	if (PlayableParkourData)
		PlayMontage();
	else
		WallClimbingTest();
}

FParkourData const* UParkourComponent::FindData(EParkourType const Type) const
{
	if (DataMap.Contains(Type) == false)
		return false;

	auto const& DataArray = DataMap[Type];

	for (int32 i = 0; i < DataArray.Num(); i++)
	{
		// UKismetMathLibrary::InRange_FloatFloat(Height, PData->DistMin, PData->DistMax, true, false)
		if (DataArray[i].DistMin <= Height && Height < DataArray[i].DistMax) // Simple Calc
		{
			ObstacleGap = Height - 200.0f;
			return &DataArray[i];
		}
	}

	return nullptr;
}

void UParkourComponent::BracedDrop()
{
	PlayableParkourData = FindData(EParkourType::Climb);

	if (PlayableParkourData)
		PlayMontage();
}

void UParkourComponent::PlayMontage()
{
	if (PlayableParkourData)
	{
		if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
		{
			OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
			IgnoreInput(true, false);
			if (auto const& OwnerAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
				OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::ResetValues);

			OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
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
		ObstacleGap = 0.0f;
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



