#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "InteractableNPC.generated.h"

class UDataCraftNPC;
class UWrapBox;
/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API AInteractableNPC : public AInteractableObjects
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void InteractObject() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UUserWidget* UIInteractableNPC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> UIInteractableNPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	TArray<UDataCraftNPC*> DataAssets;
	
	bool IsOpen = false;
};
