#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem.generated.h"

class AGameNotificationManager;
class USlotButtonInventory;
class UWidgetComponent;
class AItem;
class UWrapBox;
class UInventoryDataItems;
class UImage;
class UBorder;
class UButton;
class UHorizontalBox;
class UTextBlock;

UENUM()
enum EItemType
{
	Ed_I UMETA(DisplayName="Item"),
	Ed_W UMETA(DisplayName="Weapon"),
	Ed_Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FButtonWithID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ButtonID;

	FButtonWithID() : Button(nullptr), ButtonID(-1) {}
};

UCLASS()
class REAPTHEUNDEAD_API AInventorySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventorySystem();
	void InteractInventory();
	void UseSlots(int Index);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void AddItem(UInventoryDataItems* ItemData, int Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void RemoveItem(UInventoryDataItems* ItemData, int Amount = 1);

	TArray<UInventoryDataItems*> GetDataAssets();
	TArray<UInventoryDataItems*> GetAllDataAssets();
	void SaveInventoryToFile();
	void LoadInventoryFromFile();
protected:
	virtual void BeginPlay() override;
	static EItemType GetItemType(EItemType ItemType);

	int ID;
	int Quantity;
	UTexture* Image;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<USlotButtonInventory*> ImagesButtonsInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	int SlotsUsedMainInvetory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	int SlotsUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UInventoryDataItems*> DataAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UInventoryDataItems*> AllDataAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UUserWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UImage* ImageBtnCloseInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<int, UTexture*> DefaultSlotImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UBorder* InventoryBorder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UButton*> ButtonsSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<int, UClass*> InventorySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<UClass*, AItem*> InstanciatedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UTextBlock* CountSlotVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	AGameNotificationManager* GameNotificationManager;
	
	bool IsOpen = false;
	bool IsFirstDoubleClick = true;
	bool ItemSelected = false;
	int StockIndexSelected;
	int TotalUsedSlots;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void CloseInventory();

	void LoadInventory();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void OnButtonDoubleClicked(int32 ButtonIndex);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void OnButtonClickedMainSlotInventory(int32 ButtonIndex);

	UClass* FoundClassInSlot(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void GetSlotsMainInventory(TArray<USlotButtonInventory*> Slots);

	void ShowItemsInInventorySlot(int Index);
	void ShowItemsInInventorySlot2(int Index, int Index2);

	FString SaveDirectory = FPaths::ProjectDir();
	FString SaveFilePath = SaveDirectory / TEXT("SaveData.json");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	FString CountSlot;
};
