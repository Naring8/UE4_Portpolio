#include "ParkourComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UpTrace = { 0.0f, 0.0f, 600.0f };
}

#include "UObject/ConstructorHelpers.h"
void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorsToIgnore.Add(Owner); // Owner를 무시하도록 추가

	//if (IsValid(ParkourDataTable))
	//{
	//	TArray<FParkourData const*> Rows;

	//	ParkourDataTable->GetAllRows("", Rows);	// "" 안에 들어간 내용이 들어간 문자열만 받아옴 / "" << 아무것도 없기 때문에 모두 가져옴

	//	if (Rows.Num() > 0)
	//	{
	//		for (int32 Key = 0; Key < int32(EParkourType::Max); Key++) // TODO: 
	//		{
	//			TArray<FParkourData> Values;

	//			for (auto& Row : Rows)
	//				if (Key == Row->Type)
	//					Values.Add(*Row);

	//			DataMap.Add(EParkourType(Key), Values);
	//		}
	//	}
	//}
}


void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParkourComponent::BeginParkour(const float CapsuleHalfHeight, const float CapsuleRadius)
{
	if (IsValid(Owner))
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
			Owner->GetActorForwardVector().GetSafeNormal(0.0001)));

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
	if ((Height >= 40) && (UKismetMathLibrary::InRange_FloatFloat(Rotation, 140.0f, 220.0f)))
	{
		//switch (Height)
		//{

		//default:
		//	break;
		//}()
		//	// TODO: Change Jump Montage depends on Height
	}
}

#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
void UParkourComponent::Vaulting()
{
	if (IsValid(ParkourDataTable))
	{
		if (FParkourData const* Temp = FindData())
		{
			if (auto const& OwnerCharacter = Cast<ACharacter>(Owner))
			{
				//OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				OwnerCharacter->PlayAnimMontage(Temp->Montage, Temp->PlayRate);
			}
			
		}
		else
			WallClimbingTest();
	}

}

FParkourData const* UParkourComponent::FindData() const
{
	if (IsValid(ParkourDataTable))
	{
		TArray<FParkourData const*> PArray;
		ParkourDataTable->GetAllRows("", PArray);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Valid"));
		for (auto const& data : PArray)
			if (UKismetMathLibrary::InRange_FloatFloat(Height, data->DistMin, data->DistMax, true, false))
				return data;
	}

	return nullptr;
}

