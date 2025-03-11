#include "AnimationDataAsset.h"

FMontageData UMontageDataAsset::GetMontageData(int32 const Index)
{
	FMontageData MontageDatum;

	if (MontageData.IsValidIndex(Index))
		MontageDatum = MontageData[Index];

	return MontageDatum;
}

FSequenceData USequenceDataAsset::GetSequenceData(int32 const Index)
{
	FSequenceData SequenceDatum;

	if (SequenceData.IsValidIndex(Index))
		SequenceDatum = SequenceData[Index];

	return SequenceDatum;
}
