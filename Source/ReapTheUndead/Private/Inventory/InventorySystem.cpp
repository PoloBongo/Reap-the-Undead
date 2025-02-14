#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Inventory/Item.h"
#include "Inventory/SlotButtonInventory.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "Notification/GameNotificationManager.h"

AInventorySystem::AInventorySystem(): ID(0), Quantity(0), Image(nullptr), SlotsUsedMainInvetory(0), SlotsUsed(0),
                                      InventoryWidget(nullptr),
                                      InventoryWrapBox(nullptr),
                                      ImageBtnCloseInventory(nullptr),
                                      InventoryBorder(nullptr), CountSlotVariable(nullptr),
                                      GameNotificationManager(nullptr), StockIndexSelected(0),
                                      TotalUsedSlots(0)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	if (!InventoryWidget) return;
	InventoryWidget->AddToViewport();

	LoadInventoryFromFile();
}

void AInventorySystem::OnButtonDoubleClicked(int32 ButtonIndex)
{
	bool FoundDoubleItem = false;
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
                for (UInventoryDataItems* DataAsset : AllDataAssets)
                {
                    if (DataAsset->ItemClass == Found)
                    {
                        SaveActualAssetData = DataAsset;
                        break;
                    }
                }

                if (SaveActualAssetData)
                {
                    if (DefaultSlotImage.Num() > 0 && Button)
                    {
                        ButtonStyle.Normal.SetResourceObject(DefaultSlotImage[SaveActualAssetData->UsedSlot]);
                        ButtonStyle.Hovered.SetResourceObject(DefaultSlotImage[SaveActualAssetData->UsedSlot]);
                    }

                    Button->SetStyle(ButtonStyle);
                    InventorySlots.Remove(ButtonIndex);
                    SaveActualAssetData->Quantity++;
                    SaveActualAssetData->InMainInventory = true;
                    SaveActualAssetData->UsedSlotMainInventory = TotalUsedSlots;

                	UInventoryDataItems* FoundAllDataAsset = nullptr;
                	for (UInventoryDataItems* DataAsset : DataAssets)
                	{
                		if (DataAsset && SaveActualAssetData)
                		{
                			FoundDoubleItem = true;
                			break;
                		}
                	}
                	
                	if (!FoundDoubleItem) DataAssets.Add(SaveActualAssetData);

                    GameNotificationManager->SetTextNotification(FString::Printf(TEXT("%s a bien été supprimé du raccourcie %d"), *SaveActualAssetData->Image->GetName(), ButtonIndex), FColor::Green);
                    LoadInventory();
                }
                else
                {
                    GameNotificationManager->SetTextNotification(TEXT("L'élément à remettre dans l'inventaire n'a pas été trouvé !"), FColor::Red);
                }
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
	TotalUsedSlots = 0;
	
	TArray<UInventoryDataItems*> ValidItems;
	for (UInventoryDataItems* Item : DataAssets)
	{
		if (Item->Quantity > 0 && Item->InMainInventory)
		{
			ValidItems.Add(Item);
		}
	}

	for (UInventoryDataItems* Data : ValidItems)
	{
		if (Index < ImagesButtonsInventory.Num())
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

			Data->UsedSlotMainInventory = Index;
			Index++;
			TotalUsedSlots++;
		}
	}
	SlotsUsedMainInvetory = TotalUsedSlots;
	CountSlot = FString::Printf(TEXT("%d/%d"), TotalUsedSlots, ImagesButtonsInventory.Num());
}

void AInventorySystem::AddItem(UInventoryDataItems* ItemData, int Amount)
{
    if (DataAssets.Find(ItemData))
    {
        ItemData->Quantity += Amount;
    }
    else
    {
    	ItemData->InMainInventory = true;
    	ItemData->UsedSlotMainInventory = SlotsUsedMainInvetory + 1;
        DataAssets.Add(ItemData);
    }

	ItemData->InInventory = true;

    LoadInventory();
}

void AInventorySystem::RemoveItem(UInventoryDataItems* ItemData, int Amount)
{
    if (DataAssets.Find(ItemData))
    {
        ItemData->Quantity -= Amount;
    	
        if (ItemData->Quantity <= 0)
        {
        	ItemData->InInventory = false;
            DataAssets.Remove(ItemData);
        	SlotsUsedMainInvetory--;
        }
    }

    LoadInventory();
}

void AInventorySystem::InteractInventory()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!InventoryWidget && !InventoryBorder && !PlayerController && !ImageBtnCloseInventory) return;

	if (IsOpen)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetIgnoreMoveInput(false);
		InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Hidden);
		CountSlotVariable->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetIgnoreMoveInput(true);
		InventoryBorder->SetVisibility(ESlateVisibility::Visible);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Visible);
		CountSlotVariable->SetVisibility(ESlateVisibility::Visible);
		IsFirstDoubleClick = true;
	}
	PlayerController->SetShowMouseCursor(!IsOpen);
	IsOpen = !IsOpen;
	SaveInventoryToFile();
}

void AInventorySystem::CloseInventory()
{
	InteractInventory();
	SaveInventoryToFile();
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
		ShowItemsInInventorySlot(Index);
		LoadInventory();
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

void AInventorySystem::ShowItemsInInventorySlot(int Index)
{
	if (InventorySlots.Find(Index))
	{
		GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Impossible de mettre %s dans le raccourcie %d car il y a déjà un item"), *DataAssets[StockIndexSelected]->Image->GetName(), StockIndexSelected), FColor::Red);
	}
	else
	{
		InventorySlots.Add(Index, DataAssets[StockIndexSelected]->ItemClass);
		DataAssets[StockIndexSelected]->StockIDInSlot = StockIndexSelected;
		DataAssets[StockIndexSelected]->InMainInventory = false;
		DataAssets[StockIndexSelected]->UsedSlot = Index;
		DataAssets[StockIndexSelected]->UsedSlotMainInventory = -1;

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
		DataAssets.RemoveAt(StockIndexSelected);
	}
}

void AInventorySystem::ShowItemsInInventorySlot2(int Index, int Index2)
{
	if (!InventorySlots.Find(Index))
	{
		InventorySlots.Add(Index, DataAssets[Index2]->ItemClass);
		DataAssets[Index2]->InMainInventory = false;
		DataAssets[Index2]->UsedSlot = Index;
		DataAssets[Index2]->UsedSlotMainInventory = -1;

		FButtonStyle ButtonStyle = ButtonsSlots[Index2]->GetStyle();
			
		ButtonStyle.Normal.SetResourceObject(DataAssets[Index2]->Image);
		ButtonStyle.Hovered.SetResourceObject(DataAssets[Index2]->Image);
		ButtonStyle.Pressed.SetResourceObject(DataAssets[Index2]->Image);
			
		FVector2D ImageSize(64.f, 64.f);
		ButtonStyle.Normal.SetImageSize(ImageSize);
		ButtonStyle.Hovered.SetImageSize(ImageSize);
		ButtonStyle.Pressed.SetImageSize(ImageSize);

		ButtonsSlots[Index]->SetStyle(ButtonStyle);
		ItemSelected = false;
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

TArray<UInventoryDataItems*> AInventorySystem::GetDataAssets()
{
	return DataAssets;
}

TArray<UInventoryDataItems*> AInventorySystem::GetAllDataAssets()
{
	return AllDataAssets;
}

void AInventorySystem::SaveInventoryToFile()
{
	TArray<TSharedPtr<FJsonValue>> JsonItems;

	for (UInventoryDataItems* Item : AllDataAssets)
	{
		if (Item->InInventory)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			JsonObject->SetNumberField(TEXT("ID"), Item->ID);
			JsonObject->SetNumberField(TEXT("Quantity"), Item->Quantity);
			JsonObject->SetNumberField(TEXT("UsedSlotMainInventory"), Item->UsedSlotMainInventory);
			JsonObject->SetNumberField(TEXT("UsedSlot"), Item->UsedSlot);
			JsonObject->SetBoolField(TEXT("InMainInventory"), Item->InMainInventory);
			JsonObject->SetBoolField(TEXT("InInventory"), Item->InInventory);
			JsonObject->SetNumberField(TEXT("StockIDInSlot"), Item->StockIDInSlot);

			JsonItems.Add(MakeShareable(new FJsonValueObject(JsonObject)));
		}
	}
	TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());
	RootObject->SetArrayField(TEXT("Items"), JsonItems);

	FString OutputString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);
	
	FFileHelper::SaveStringToFile(OutputString, *SaveFilePath);
	
	UE_LOG(LogTemp, Warning, TEXT("Save File Path: %s"), *SaveFilePath);
}

void AInventorySystem::LoadInventoryFromFile()
{
    DataAssets.Empty();
    FString FileContent;
    
    if (FFileHelper::LoadFileToString(FileContent, *SaveFilePath))
    {
        TSharedPtr<FJsonObject> RootObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContent);

        if (FJsonSerializer::Deserialize(JsonReader, RootObject) && RootObject.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>& JsonItems = RootObject->GetArrayField(TEXT("Items"));

            if (JsonItems.Num() > 0)
            {
                for (const TSharedPtr<FJsonValue>& JsonValue : JsonItems)
                {
                    TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();

                    int32 ItemID = JsonObject->GetNumberField(TEXT("ID"));

                    UInventoryDataItems* FoundAllDataAsset = nullptr;
                    for (UInventoryDataItems* DataAsset : AllDataAssets)
                    {
                        if (DataAsset && DataAsset->ID == ItemID)
                        {
                            FoundAllDataAsset = DataAsset;
                            break;
                        }
                    }

                    if (FoundAllDataAsset)
                    {
                        FoundAllDataAsset->Quantity = JsonObject->GetNumberField(TEXT("Quantity"));
                        FoundAllDataAsset->UsedSlotMainInventory = JsonObject->GetNumberField(TEXT("UsedSlotMainInventory"));
                        FoundAllDataAsset->UsedSlot = JsonObject->GetNumberField(TEXT("UsedSlot"));
                        FoundAllDataAsset->InMainInventory = JsonObject->GetBoolField(TEXT("InMainInventory"));
                        FoundAllDataAsset->InInventory = JsonObject->GetBoolField(TEXT("InInventory"));
                        FoundAllDataAsset->StockIDInSlot = JsonObject->GetNumberField(TEXT("StockIDInSlot"));

                    	DataAssets.Add(FoundAllDataAsset);
                        if (!FoundAllDataAsset->InMainInventory)
                        {
                            ShowItemsInInventorySlot2(FoundAllDataAsset->UsedSlot, FoundAllDataAsset->StockIDInSlot);
                        }
                    }
                }
            }

        	LoadInventory();
        }
    }
}