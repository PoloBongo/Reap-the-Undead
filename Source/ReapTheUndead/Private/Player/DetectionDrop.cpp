#include "Player/DetectionDrop.h"

#include "Components/BoxComponent.h"

UDetectionDrop::UDetectionDrop(): DropItem(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDetectionDrop::BeginPlay()
{
	Super::BeginPlay();

}

void UDetectionDrop::SetDropItem(ADropItem* DropItemRef)
{
	DropItem = DropItemRef;
}
