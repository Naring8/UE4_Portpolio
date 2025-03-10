#include "AssassinationComponent.h"
#include <GameFramework/Character.h>
#include <Components/ArrowComponent.h>

UAssassinationComponent::UAssassinationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UAssassinationComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UAssassinationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (auto const& OwnerCharacter = Cast<ACharacter>(GetOwner()))
		TraceForward();
}

void UAssassinationComponent::Assassinate()
{
}

void UAssassinationComponent::TraceForward()
{
}
