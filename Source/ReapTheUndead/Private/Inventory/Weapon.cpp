#include "Inventory/Weapon.h"

#include "Inventory/DataAsset/InventoryDataItems.h"

AWeapon::AWeapon(): ItemType(Ed_W), Damage(0), DataAsset(nullptr) {}

void AWeapon::BeginPlay()
{
	if (DataAsset)
	{
		ID = DataAsset->ID;
		Quantity = DataAsset->Quantity;
	}
}

void AWeapon::UseWeapon()
{
	if (DataAsset->Quantity <= 0) return;
	DataAsset->Quantity--;

	UE_LOG(LogTemp, Error, TEXT("ID : %d"), ID);
	UE_LOG(LogTemp, Error, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Error, TEXT("Quantity : %d"), Quantity);
	UE_LOG(LogTemp, Error, TEXT("Damage : %f"), Damage);
}
