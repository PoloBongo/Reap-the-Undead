#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataCraftNPC.generated.h"

class UInventoryDataItems;
class AItem;

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API UDataCraftNPC : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int AmountToAdd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UInventoryDataItems*> Datas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UInventoryDataItems* RewardItem;
};


