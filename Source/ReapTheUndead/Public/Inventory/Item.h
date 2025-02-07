#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySystem.h"
#include "Item.generated.h"

UENUM(BlueprintType)
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

	void UseItem();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constructor", meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EItemType> ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constructor", meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EItemUsable> ItemUsable;
	
};
