#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DetectionDrop.generated.h"


class ADropItem;
class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REAPTHEUNDEAD_API UDetectionDrop : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDetectionDrop();

protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION(BlueprintCallable)
	void SetDropItem(ADropItem* DropItemRef);
	
	ADropItem* DropItem;
};
