#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySystem.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API AWeapon : public AInventorySystem
{
	GENERATED_BODY()

public:
	AWeapon();
	AWeapon(int ID, EItemType Type, int Quantity, float Damage);

	void UseWeapon();
private:
	EItemType ItemType;
	float Damage;
	
};
