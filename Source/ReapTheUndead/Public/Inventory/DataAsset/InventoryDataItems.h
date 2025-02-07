#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryDataItems.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API UInventoryDataItems : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UClass* ItemClass;
};


