#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"

AInventorySystem::AInventorySystem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	
}	

void AInventorySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AInventorySystem::InteractInventory()
{
	if (!InventoryWidget) return;
	IsOpen ? InventoryWidget->RemoveFromParent() : InventoryWidget->AddToViewport();
	IsOpen = !IsOpen;
}

EItemType AInventorySystem::GetItemType(const EItemType ItemType)
{
	return ItemType;
}

void AInventorySystem::AddObjectInInventory(UObject* Object)
{
	Objects.Add(Object);
}
