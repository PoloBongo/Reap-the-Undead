#include "Inventory/Weapons.h"

#include "Inventory/DataAsset/InventoryDataItems.h"

AWeapons::AWeapons() {}

void AWeapons::BeginPlay()
{
	if (DataAsset)
	{
		ID = DataAsset->ID;
		Quantity = DataAsset->Quantity;
	}
}

void AWeapons::UseItem()
{
	if (DataAsset->Quantity <= 0) return;
	DataAsset->Quantity--;

	UE_LOG(LogTemp, Warning, TEXT("ID : %d"), DataAsset->ID);
	UE_LOG(LogTemp, Warning, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Warning, TEXT("Quantity : %d"), DataAsset->Quantity);
	UE_LOG(LogTemp, Warning, TEXT("Usable : %d"), ItemUsable);
	UE_LOG(LogTemp, Warning, TEXT("BOINGOOOO"));
}