// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <../Basic/CustomStruct.h>
#include "AnimationDataAsset.generated.h"

UCLASS()
class UE4_PORTPOLIO_API UMontageDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	struct FMontageData GetMontageData(int32 const Index);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		TArray<struct FMontageData> MontageData;
};

UCLASS()
class UE4_PORTPOLIO_API USequenceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	struct FSequenceData GetSequenceData(int32 const Index);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		TArray<struct FSequenceData> SequenceData;
};