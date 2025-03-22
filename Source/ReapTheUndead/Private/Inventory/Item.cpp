#include "Inventory/Item.h"

#include "Inventory/DataAsset/InventoryDataItems.h"

AItem::AItem(): DataAsset(nullptr), ItemType(Ed_I), ItemUsable(Ed_N) {}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	if (DataAsset)
	{
		ID = DataAsset->ID;
		Quantity = DataAsset->Quantity;
	}
}

void AItem::UseItem()
{
	if (DataAsset->Quantity <= 0) return;
	DataAsset->Quantity--;
	
	FString Message = FString::Printf(TEXT("Item utilisé : %s"), *DataAsset->Name);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
}
