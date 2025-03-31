#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	IDLE,
	DAGGER,
	MAX
};

UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class UE4_PORTPOLIO_API IWeaponInterface
{
	GENERATED_BODY()

public:
#pragma region WeaponStateInterface
	virtual void EnableAttack()  PURE_VIRTUAL(IWeaponInterface::EnableAttack)
		virtual void DisableAttack() PURE_VIRTUAL(IWeaponInterface::DisableAttack)
#pragma endregion
#pragma region WeaponActionInterface
		virtual void ActivateAttack() PURE_VIRTUAL(IWeaponInterface::ActivateAttack)
		virtual void DeactivateAttack() PURE_VIRTUAL(IWeaponInterface::DeactivateAttack)
#pragma endregion
};
