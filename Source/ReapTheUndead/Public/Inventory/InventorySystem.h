#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem.generated.h"

class AItem;
class UWrapBox;
class UInventoryDataItems;
class UImage;
class UBorder;
class UButton;
class UHorizontalBox;

UENUM()
enum EItemType
{
	Ed_I UMETA(DisplayName="Item"),
	Ed_W UMETA(DisplayName="Weapon"),
	Ed_Max UMETA(Hidden)
};

UCLASS()
class REAPTHEUNDEAD_API AInventorySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventorySystem();
	virtual void Tick(float DeltaTime) override;
	void InteractInventory();
	void UseSlots(int Index);

protected:
	virtual void BeginPlay() override;
	static EItemType GetItemType(EItemType ItemType);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void AddItem(UInventoryDataItems* ItemData);
	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void RemoveItem(UInventoryDataItems* ItemData);

	int ID;
	int Quantity;
	UTexture* Image;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UClass*> Classes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UInventoryDataItems*> DataAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UUserWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UImage* ImageBtnCloseInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UBorder* InventoryBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UButton*> ButtonsSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<int, UClass*> InventorySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<UClass*, AItem*> InstanciatedItems;

	void UpdateInventorySlotImage();
	
	bool IsOpen = false;
	bool DoublonFound = false;

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void CloseInventory();

	UFUNCTION()
	void OnItemClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void LoadInventory();
};
