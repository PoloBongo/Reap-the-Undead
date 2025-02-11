#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Inventory/Item.h"
#include "Inventory/SlotButtonInventory.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "Notification/GameNotificationManager.h"

AInventorySystem::AInventorySystem(): ID(0), Quantity(0), Image(nullptr), SlotsUsedMainInvetory(0), SlotsUsed(0),
                                      InventoryWidget(nullptr),
                                      InventoryWrapBox(nullptr),
                                      ImageBtnCloseInventory(nullptr),
                                      InventoryBorder(nullptr), GameNotificationManager(nullptr), StockIndexSelected(0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	FVector SpawnLocation(0.f, 0.f, 0.f);
	FRotator SpawnRotation(0.f, 0.f, 0.f);
	
	GameNotificationManager = GetWorld()->SpawnActor<AGameNotificationManager>(GameNotificationManagerClass, SpawnLocation, SpawnRotation);

	if (!GameNotificationManagerClass) return;
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
				
				if (DefaultSlotImage.Num() > 0 && Button)
				{
					ButtonStyle.Normal.SetResourceObject(DefaultSlotImage[SaveActualAssetData->UsedSlot]);
					ButtonStyle.Hovered.SetResourceObject(DefaultSlotImage[SaveActualAssetData->UsedSlot]);
				}

				Button->SetStyle(ButtonStyle);
				InventorySlots.Remove(ButtonIndex);
				SaveActualAssetData->Quantity++;
				SaveActualAssetData->InMainInventory = true;

				GameNotificationManager->SetTextNotification(FString::Printf(TEXT("%s a bien été supprimé du raccourcie %d"), *SaveActualAssetData->Image->GetName(), SaveActualAssetData->UsedSlot), FColor::Green);

				LoadInventory();
			}
		}
	}

	LastClickTime = CurrentTime;
}

void AInventorySystem::OnButtonClickedMainSlotInventory(int32 ButtonIndex)
{
	for (int i = 0; i < DataAssets.Num(); i++)
	{
		if (ButtonIndex == i)
		{
			StockIndexSelected = i;
			ItemSelected = true;
			GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Vous avez sélectionné l'item suivant: %s"), *DataAssets[i]->Image->GetName()), FColor::Cyan);
			break;
		}
	}
	LoadInventory();
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
    	Data->UsedSlotMainInventory = i;
    	
        if (Data->Quantity > 0 && Index < ImagesButtonsInventory.Num() && Data->InMainInventory)
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
                    	
                    	UE_LOG(LogTemp, Warning, TEXT("data assets %d"), DataAssets.Num());
                    	UE_LOG(LogTemp, Warning, TEXT("i %d"), i);
                    }
                }
            }
            Index++;
        	SlotsUsedMainInvetory = DataAssets.Num();
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
    	ItemData->UsedSlotMainInventory = SlotsUsedMainInvetory + 1;
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
        	SlotsUsedMainInvetory--;
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
		PlayerController->SetInputMode(FInputModeGameAndUI());
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

	if (ItemSelected)
	{
		if (InventorySlots.Find(Index))
		{
			GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Impossible de mettre %s dans le raccourcie %d car il y a déjà un item"), *DataAssets[StockIndexSelected]->Image->GetName(), StockIndexSelected), FColor::Red);
		}
		else
		{
			InventorySlots.Add(Index, DataAssets[StockIndexSelected]->ItemClass);
			DataAssets[StockIndexSelected]->InMainInventory = false;
			DataAssets[StockIndexSelected]->UsedSlot = Index;

			FButtonStyle ButtonStyle = ButtonsSlots[StockIndexSelected]->GetStyle();
			
			ButtonStyle.Normal.SetResourceObject(DataAssets[StockIndexSelected]->Image);
			ButtonStyle.Hovered.SetResourceObject(DataAssets[StockIndexSelected]->Image);
			ButtonStyle.Pressed.SetResourceObject(DataAssets[StockIndexSelected]->Image);
			
			FVector2D ImageSize(64.f, 64.f);
			ButtonStyle.Normal.SetImageSize(ImageSize);
			ButtonStyle.Hovered.SetImageSize(ImageSize);
			ButtonStyle.Pressed.SetImageSize(ImageSize);

			ButtonsSlots[Index]->SetStyle(ButtonStyle);
			ItemSelected = false;

			GameNotificationManager->SetTextNotification(FString::Printf(TEXT("%s est maintenant sur le raccourcie %d"), *DataAssets[StockIndexSelected]->Image->GetName(), StockIndexSelected), FColor::Green);

			LoadInventory();
		}
	}
	else
	{
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
					SlotsUsed++;
				}
			}

			if (SpawnedItem)
			{
				SpawnedItem->UseItem();
			}
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
