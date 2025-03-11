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
struct FAssassinationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TEnumAsByte<EAssassination::Type> Type;
	UPROPERTY(EditAnywhere) class UAnimSequence* Sequence;
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
	UFUNCTION(BlueprintCallable)
		void Assassinate();

private:
	void TraceForward();
		
private:
	UPROPERTY()
		TArray<struct FAssassinationData> AssassinationArray;

	class UArrowComponent* ForwardArrow;
	float TraceDistance = 50.0f;
};
