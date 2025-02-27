#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPlayerController.generated.h"

UCLASS()
class UE4_PORTPOLIO_API ABasicPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetupInputComponent() override;

private:
	void MoveFB(const float Value);
	void MoveLR(const float Value);
	void LookUD(const float Value);
	void LookLR(const float Value);

	void Run(const float Value);
};
