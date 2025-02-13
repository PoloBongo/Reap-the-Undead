#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryDataItems.generated.h"

class AItem;

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API UInventoryDataItems : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UClass* ItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int UsedSlotMainInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int UsedSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool InMainInventory = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool InInventory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int StockIDInSlot;
};


