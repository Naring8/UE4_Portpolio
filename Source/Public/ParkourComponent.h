#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/DataTable.h> // To Use DateTable in Blueprint
#include "ParkourComponent.generated.h"

UENUM(BlueprintType)
namespace EParkour
{
	enum Type
	{
		None,
		Jump,
		Vault,
		Climb,
		Fall,
		Slide,
		Max
	};
} typedef EParkour::Type EParkourType;

USTRUCT(Atomic, BlueprintType)
struct FParkourData : public FTableRowBase // inheritance is essencial to use "DataTable" in [Blueprint]
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TEnumAsByte<EParkour::Type> Type;
	UPROPERTY(EditAnywhere) class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere) float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere) FName Section;
	UPROPERTY(EditAnywhere) float DistMin;
	UPROPERTY(EditAnywhere) float DistMax;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTPOLIO_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(BlueprintCallable)
		void BeginParkour(const float CapsuleHalfHeight, const float CapsuleRadius);

private:
#pragma region TraceSide
	void TraceForward();
	void CheckObstacleHeight(FHitResult HitResult);
	void CheckObstacleThickness(FHitResult HitResult);
	void WallClimbingTest();
	void Jumping();
	void Vaulting();

	bool FindData();
	void PlayMontage();
	UFUNCTION()
		void ResetValues(UAnimMontage* Montage, bool bInterrupted);
#pragma endregion
	void IgnoreInput(bool LookInput, bool MoveInput);

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
		class UDataTable* ParkourDataTable;

private:
	class AActor* Owner;
	UPROPERTY()
		TArray<class AActor*> ActorsToIgnore;

private:
	float OwnerCapsuleHalfHeight = 0.0f;
	float OwnerCapsuleRadius = 0.0f;

	FParkourData const* PlayableParkourData;

private:
	FVector SideLocation;
	FVector SideNormal;
	float Rotation;

	FVector UpTrace;

	FVector SidePlace;
	float Height;

	bool HasSide;
};
