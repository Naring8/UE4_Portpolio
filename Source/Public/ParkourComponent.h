#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTPOLIO_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(BlueprintCallable)
		void BeginParkour(const float CapsuleHalfHeight, const float CapsuleRadius);

private:
#pragma region TraceSide
	void TraceForward();
	void CheckObstacleHeight(FHitResult HitResult);
	void CheckObstacleThickness(FHitResult HitResult);
	void WallClimbingTest();
	void Jumping();
	void Vaulting();
#pragma endregion


private:
	UPROPERTY()
		TWeakObjectPtr<class AActor> Owner;

	TArray<AActor*> ActorsToIgnore;

private:
	float OwnerCapsuleHalfHeight = 0.0f;
	float OwnerCapsuleRadius = 0.0f;

private:
	FVector SideLocation;
	FVector SideNormal;
	float Rotation;

	FVector UpTrace;

	FVector SidePlace;
	float Height;

	bool HasSide;
};
