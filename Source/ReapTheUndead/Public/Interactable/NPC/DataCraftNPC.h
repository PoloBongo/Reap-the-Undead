#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataCraftNPC.generated.h"

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
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UClass* ItemClass;
};


