#include "Inventory/Weapon.h"

AWeapon::AWeapon() {}

AWeapon::AWeapon(int ID, EItemType Type, int Quantity, float Damage)
{
	AddObjectInInventory(this->GetClass());
}

void AWeapon::UseWeapon()
{
	UE_LOG(LogTemp, Error, TEXT("ID : %d"), ID);
	UE_LOG(LogTemp, Error, TEXT("Type : %d"), ItemType);
	UE_LOG(LogTemp, Error, TEXT("Quantity : %d"), Quantity);
	UE_LOG(LogTemp, Error, TEXT("Damage : %f"), Damage);
}
