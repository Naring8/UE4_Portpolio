#include "MontageDataAsset.h"

FMontageData UMontageDataAsset::GetMontageData(int32 const Index)
{
	FMontageData MontageDatum;

	if (MontageData.IsValidIndex(Index))
		MontageDatum = MontageData[Index];

	return MontageDatum;
}
