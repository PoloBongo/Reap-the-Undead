#include "Inventory/Weapons.h"

#include "Inventory/DataAsset/InventoryDataItems.h"

AWeapons::AWeapons(): AttackTriggerParent(nullptr)
{
}

void AWeapons::BeginPlay()
{
	Super::BeginPlay();
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
	
	OnPlayAttackKamea.Broadcast();
}