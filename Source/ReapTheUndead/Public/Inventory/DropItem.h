#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "DropItem.generated.h"

class UBorder;
class AGameNotificationManager;
class AReapTheUndeadCharacter;
class UDetectionDrop;
class UButton;
class AInventorySystem;
class USlotButtonInventory;
class UInventoryDataItems;
class UWrapBox;
/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API ADropItem : public AInteractableObjects
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FOnButtonClickedDelegate, int, ID);

	void AddItemToDrop(UInventoryDataItems* InventoryDataItemsVar);
	UInventoryDataItems* GetItemDropAt(UInventoryDataItems* InventoryDataItems);
protected:
	ADropItem();

	virtual void InteractObject() override;
	virtual void Destroyed() override;
	virtual void BeginPlay() override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;
private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	UUserWidget* WidgetStashDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	UWrapBox* InventoryDropWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	TMap<int, UInventoryDataItems*> DropListItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	TArray<USlotButtonInventory*> ImagesButtonsDropInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	TSubclassOf<USlotButtonInventory> ImagesButtonsDropInventoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	AInventorySystem* InventorySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	FString CountDropSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	AReapTheUndeadCharacter* ReapTheUndeadCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	AGameNotificationManager* GameNotificationManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	int MaxSlots = 10;
	
	void InteractDropInventory();
	void LoadDropInventory();

	UFUNCTION(BlueprintCallable)
	void CloseDropInventory(APlayerController* PlayerControllerVar, bool Close = false);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void OnButtonDoubleClicked(int ButtonIndex);

	bool IsOpen = false;
	int TotalUsedDropSlots = 0;
	FTimerHandle AnimationHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	bool IsDrop = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Inventory", meta=(AllowPrivateAccess="true"))
	float TimeAnimation = 2.f;

	UFUNCTION(BlueprintCallable, Category = "Button Delegate")
	void TriggerButtonClickedDelegate(int ButtonID);
	void CheckDestroyingDrop();
	void StopAnimation();

	/* Hovered Part */
	UFUNCTION(BlueprintCallable, Category = "Button Delegate")
	void TriggerButtonHoveredDelegate(int ButtonID);
	
	UFUNCTION(BlueprintCallable, Category = "Button Delegate")
	void TriggerButtonUnHoveredDelegate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hovered Property", meta=(AllowPrivateAccess="true"))
	UUserWidget* HoveredWidget;

	void AttachHoveredEvent() const;
	void AttachUnHoveredEvent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hovered Property", meta=(AllowPrivateAccess="true"))
	FString QuantityText;

	TMap<UInventoryDataItems*, int> DropQuantityInstance;
};
