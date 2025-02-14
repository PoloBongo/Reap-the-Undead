#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "InteractableNPC.generated.h"

class AGameNotificationManager;
class UBorder;
class UInventoryDataItems;
class AInventorySystem;
class UButton;
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
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UButton* SendButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UButton* ArrowLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UButton* ArrowRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UBorder* CraftBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	TArray<UDataCraftNPC*> CraftAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	AInventorySystem* InventorySystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	FString CraftingName = "Crafter";
private:
	bool IsOpen = false;
	int IndexCraft = 0;
	int MaxIndexCraft;

	void SetInputModeNPC() const;

	UFUNCTION()
	void CraftingCheck();
	UFUNCTION()
	void GoLeft();
	UFUNCTION()
	void GoRight();

	void LoadCraft();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	TArray<UInventoryDataItems*> StocksPlacementItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta=(AllowPrivateAccess="true"))
	USoundWave* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	AGameNotificationManager* Notification;
};
