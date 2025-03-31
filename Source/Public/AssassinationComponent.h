#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/DataTable.h>
#include "AssassinationComponent.generated.h"

UENUM(BlueprintType)
namespace EAssassination
{
	enum Type
	{
		Assassin,
		Assassinated,
		Max
	};
} typedef EAssassination::Type EAssassinationType;

USTRUCT(Atomic, BlueprintType)
struct FAssassinateData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TEnumAsByte<EAssassination::Type> Type;
	UPROPERTY(EditAnywhere) class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere) float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere) FName Section;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTPOLIO_API UAssassinationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAssassinationComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
		void TraceForward();

	UFUNCTION(BlueprintCallable)
		void Assassinate();

	void SetCharacterWidget(class AActor* const Actor, bool const Detect);

	UFUNCTION()
		void SetIdle(UAnimMontage* const Montage, bool const bInterrupted);
	UFUNCTION()
		void DeadAfterAnimation(UAnimMontage* const Montage, bool const bInterrupted);

private:
	UPROPERTY(EditAnywhere, category = "DataTable", meta = (AllowPrivateAccess = "true"))
		class UDataTable* AssassinateDataTable;
	TMap<EAssassinationType, FAssassinateData> DataMap;

	class UArrowComponent* ForwardArrow;

	float TraceDistance = 180.0f;

private:
	AActor* HitObstacle = nullptr;
	FVector HitObstacleExtent = FVector::ZeroVector;
	float HitDistance = 0;
	float YawToFace = 0;

	FHitResult HitResult;

	FVector WallLocation = FVector::ZeroVector;
	FVector WallNormal = FVector::ZeroVector;

	bool bCanTrace = true;
	FAssassinateData const* Player;
	FAssassinateData const* Enemy;
};
