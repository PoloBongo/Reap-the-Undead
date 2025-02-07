#include "Inventory/Item.h"

AItem::AItem(): ItemType(Ed_I), ItemUsable(Ed_N)
{	
}

void AItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ID : %d"), ID);
	UE_LOG(LogTemp, Warning, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Warning, TEXT("Quantity : %d"), Quantity);
	UE_LOG(LogTemp, Warning, TEXT("Usable : %d"), ItemUsable);

	if (Quantity <= 0) return;
	Quantity--;

	
}
