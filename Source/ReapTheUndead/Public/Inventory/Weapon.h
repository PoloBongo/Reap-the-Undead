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

	void UseWeapon();
	
protected:
	virtual void BeginPlay() override;
	
private:
	EItemType ItemType;
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UInventoryDataItems* DataAsset;
};
