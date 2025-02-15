#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "DropItem.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API ADropItem : public AInteractableObjects
{
	GENERATED_BODY()

protected:
	ADropItem();

	virtual void InteractObject() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BagMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	UWidgetComponent* WidgetStashDropComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> WidgetStashDropClass;

	UUserWidget* WidgetStashDrop;
};
