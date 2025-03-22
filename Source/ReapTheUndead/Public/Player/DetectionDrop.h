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

	UFUNCTION(BlueprintCallable)
	void SetDropItem(ADropItem* DropItemRef = nullptr);
	ADropItem* GetDropItem() const;
protected:
	virtual void BeginPlay() override;
private:
	ADropItem* DropItem;
};
