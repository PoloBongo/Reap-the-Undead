#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "ShopItem.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API AShopItem : public AInteractableObjects
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void InteractObject() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UUserWidget* UIInteractableNPC;

private:
	bool IsOpen = false;
};
