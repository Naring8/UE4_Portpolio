#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CustomStruct.generated.h"

UCLASS()
class UE4_PORTPOLIO_API UCustomStruct : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic, BlueprintType)
struct FMontageData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UAnimMontage* Montage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float PlayRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName Section;
};
