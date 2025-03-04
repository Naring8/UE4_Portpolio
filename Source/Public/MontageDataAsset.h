// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MontageDataAsset.generated.h"

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


UCLASS()
class UE4_PORTPOLIO_API UMontageDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FMontageData GetMontageData(int32 const Index);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		TArray<FMontageData> MontageData;
};
