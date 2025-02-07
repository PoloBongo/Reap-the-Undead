#include "Inventory/Item.h"

#include "Inventory/DataAsset/InventoryDataItems.h"

AItem::AItem(): ItemType(Ed_I), ItemUsable(Ed_N), DataAsset(nullptr) {}

void AItem::BeginPlay()
{
	if (DataAsset)
	{
		ID = DataAsset->ID;
		Quantity = DataAsset->Quantity;
	}
}

void AItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ID : %d"), DataAsset->ID);
	UE_LOG(LogTemp, Warning, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Warning, TEXT("Quantity : %d"), DataAsset->Quantity);
	UE_LOG(LogTemp, Warning, TEXT("Usable : %d"), ItemUsable);

	if (DataAsset->Quantity <= 0) return;
	DataAsset->Quantity--;
}
