#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Inventory/Item.h"
#include "Inventory/SlotButtonInventory.h"
#include "Inventory/DataAsset/InventoryDataItems.h"

AInventorySystem::AInventorySystem(): ID(0), Quantity(0), Image(nullptr), InventoryWidget(nullptr),
                                      InventoryWrapBox(nullptr),
                                      ImageBtnCloseInventory(nullptr),
                                      DefaultSlotImage(nullptr),
                                      InventoryBorder(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	if (!InventoryWidget) return;
	InventoryWidget->AddToViewport();

	LoadInventory();
}

void AInventorySystem::OnButtonDoubleClicked(int32 ButtonIndex)
{
	UInventoryDataItems* SaveActualAssetData = nullptr;
	static float LastClickTime = 0.0f;
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	UButton* Button = ButtonsSlots[ButtonIndex];
	FButtonStyle ButtonStyle = Button->GetStyle();

	if (CurrentTime - LastClickTime < 0.3f)
	{
		if (IsFirstDoubleClick)
		{
			IsFirstDoubleClick = false;
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Double clic"));
		if (InventorySlots.Find(ButtonIndex))
		{
			if (UClass* Found = FoundClassInSlot(ButtonIndex))
			{
				for (UInventoryDataItems* DataAsset : DataAssets)
				{
					SaveActualAssetData = DataAsset;
					if (DataAsset->ItemClass->GetFName() == *Found->GetFName().ToString())
					{
						break;
					}
				}
				
				if (DefaultSlotImage && Button)
				{
					ButtonStyle.Normal.SetResourceObject(DefaultSlotImage);
					ButtonStyle.Hovered.SetResourceObject(DefaultSlotImage);
				}

				Button->SetStyle(ButtonStyle);
				InventorySlots.Remove(ButtonIndex);
				SaveActualAssetData->Quantity++;

				LoadInventory();
			}
		}
	}

	LastClickTime = CurrentTime;
}

void AInventorySystem::OnButtonClickedMainSlotInventory(int32 ButtonIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("clic %d"), ButtonIndex);
}

void AInventorySystem::LoadInventory()
{
    if (!InventoryWrapBox) return;

    for (UWidget* Child : InventoryWrapBox->GetAllChildren())
    {
        UButton* Button = Cast<UButton>(Child);
        if (Button)
        {
            Button->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    int32 Index = 0;
    for (int32 i = 0; i < DataAssets.Num(); i++)
    {
        UInventoryDataItems* Data = DataAssets[i];

        if (Data->Quantity > 0 && Index < ImagesButtonsInventory.Num())
        {
            USlotButtonInventory* ItemImageButton = ImagesButtonsInventory[Index];
            if (ItemImageButton)
            {
                FButtonStyle ButtonStyle = ItemImageButton->GetStyle();

                if (UTexture* BaseTexture = Data->Image)
                {
                    UTexture2D* ItemTexture2D = Cast<UTexture2D>(BaseTexture);
                    if (ItemTexture2D)
                    {
                        ButtonStyle.Normal.SetResourceObject(ItemTexture2D);
                        ButtonStyle.Hovered.SetResourceObject(ItemTexture2D);
                        ButtonStyle.Pressed.SetResourceObject(ItemTexture2D);

                        FVector2D ImageSize(100.f, 400.f);
                        ButtonStyle.Normal.SetImageSize(ImageSize);
                        ButtonStyle.Hovered.SetImageSize(ImageSize);
                        ButtonStyle.Pressed.SetImageSize(ImageSize);

                        ItemImageButton->SetStyle(ButtonStyle);
                        ItemImageButton->SetVisibility(ESlateVisibility::Visible); 
                    }
                }
            }
            Index++;
        }
    }
}

void AInventorySystem::AddItem(UInventoryDataItems* ItemData, int Amount)
{
    if (DataAssets.Find(ItemData))
    {
        ItemData->Quantity += Amount;
    }
    else
    {
        DataAssets.Add(ItemData);
    }

    LoadInventory();
}

void AInventorySystem::RemoveItem(UInventoryDataItems* ItemData, int Amount)
{
    if (DataAssets.Find(ItemData))
    {
        ItemData->Quantity -= Amount;

        if (ItemData->Quantity <= 0)
        {
            DataAssets.Remove(ItemData);
        }
    }

    LoadInventory();
}

void AInventorySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AInventorySystem::InteractInventory()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!InventoryWidget && !InventoryBorder && !PlayerController && !ImageBtnCloseInventory) return;

	if (IsOpen)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		InventoryBorder->SetVisibility(ESlateVisibility::Visible);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Visible);
		IsFirstDoubleClick = true;
	}
	PlayerController->SetShowMouseCursor(!IsOpen);
	IsOpen = !IsOpen;
}

void AInventorySystem::CloseInventory()
{
	InteractInventory();
}

EItemType AInventorySystem::GetItemType(const EItemType ItemType)
{
	return ItemType;
}

void AInventorySystem::UseSlots(int Index)
{
	if (!InventoryWidget) return;
	
	if (UClass* Found = FoundClassInSlot(Index))
	{
		AItem* SpawnedItem;

		if (InstanciatedItems.Contains(Found))
		{
			SpawnedItem = InstanciatedItems[Found];
		}
		else
		{
			FVector SpawnLocation(0.f, 0.f, 0.f);
			FRotator SpawnRotation(0.f, 0.f, 0.f);
                    
			SpawnedItem = GetWorld()->SpawnActor<AItem>(Found, SpawnLocation, SpawnRotation);
			if (SpawnedItem)
			{
				InstanciatedItems.Add(Found, SpawnedItem);
			}
		}

		if (SpawnedItem)
		{
			SpawnedItem->UseItem();
		}
	}
}

UClass* AInventorySystem::FoundClassInSlot(int32 Index)
{
	if (UClass** ItemClass = InventorySlots.Find(Index))
	{
		return *ItemClass;
	}

	return nullptr;
}

void AInventorySystem::GetSlotsMainInventory(TArray<USlotButtonInventory*> Slots)
{
	ImagesButtonsInventory = Slots;
}
