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
		void BeginParkour();

private:
	void TraceForward();
	void CheckObstacleHeight(FHitResult HitResult);
	void CheckObstacleThickness();

private:
	UPROPERTY()
		TWeakObjectPtr<AActor> Owner;

	TArray<AActor*> ActorsToIgnore;
};
