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

	virtual void UseItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UInventoryDataItems* DataAsset;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constructor", meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EItemType> ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constructor", meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EItemUsable> ItemUsable;
};
