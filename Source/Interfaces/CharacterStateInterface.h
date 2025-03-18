#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStateInterface.generated.h"

UENUM()
enum class ECharacterState : uint8
{
	IDLE,
	DEAD
};

UINTERFACE(MinimalAPI)
class UCharacterStateInterface : public UInterface
{
	GENERATED_BODY()
};

class UE4_PORTPOLIO_API ICharacterStateInterface
{
	GENERATED_BODY()

public:
	virtual ECharacterState GetCharacterState() PURE_VIRTUAL(ICharacterStateInterface::GetCharacterState, return ECharacterState::IDLE;)
	virtual void SetCharacterState(ECharacterState const StateType) PURE_VIRTUAL(ICharacterStateInterface::SetCharacterState)
};
