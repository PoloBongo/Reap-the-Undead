#include "Inventory/Item.h"

AItem::AItem(): ItemType(Ed_I), ItemUsable(Ed_N)
{
}

AItem::AItem(int ID, EItemType Type, int Quantity, EItemUsable Usable)
{
	UE_LOG(LogTemp, Warning, TEXT("item construct"));
	AddObjectInInventory(this);
	this->ID = 0;
	this->ItemType = Ed_I;
	this->Quantity = 1;
	this->ItemUsable = Ed_N;
}

void AItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ID : %d"), ID);
	UE_LOG(LogTemp, Warning, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Warning, TEXT("Quantity : %d"), Quantity);
	UE_LOG(LogTemp, Warning, TEXT("Usable : %d"), ItemUsable);
}
