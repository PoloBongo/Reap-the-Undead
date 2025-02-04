#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySystem.h"
#include "Item.generated.h"

UENUM()
enum EItemUsable
{
	Ed_U UMETA(DisplayName="Usable"),
	Ed_N UMETA(DisplayName="NotUsable")
};

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API AItem : public AInventorySystem
{
	GENERATED_BODY()

public:
	AItem();
	AItem(int ID, EItemType Type, int Quantity, EItemUsable Usable);

private:
	EItemType ItemType;
	EItemUsable ItemUsable;
	
	void UseItem();
};
