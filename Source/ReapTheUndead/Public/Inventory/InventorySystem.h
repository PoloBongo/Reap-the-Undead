#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem.generated.h"

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

	void AddObjectInInventory(UObject* Object);

	int ID;
	int Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Image")
	UTexture* Image;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UObject*> Objects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UUserWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UBorder* InventoryBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UButton*> ButtonsSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<int, UClass*> InventorySlots;

	void UpdateInventorySlotImage();
	
	bool IsOpen = false;
};
