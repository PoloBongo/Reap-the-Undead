#include "Inventory/Item.h"

AItem::AItem() {}

AItem::AItem(int ID, EItemType Type, int Quantity, EItemUsable Usable)
{
	AddObjectInInventory(this);
}

void AItem::UseItem()
{
	UE_LOG(LogTemp, Error, TEXT("ID : %d"), ID);
	UE_LOG(LogTemp, Error, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Error, TEXT("Quantity : %d"), Quantity);
	UE_LOG(LogTemp, Error, TEXT("Usable : %d"), ItemUsable);
}
