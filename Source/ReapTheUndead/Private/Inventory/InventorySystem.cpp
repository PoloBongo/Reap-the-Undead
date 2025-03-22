#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "GameFramework/Character.h"
#include "Inventory/DropItem.h"
#include "Inventory/Item.h"
#include "Inventory/SlotButtonInventory.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "Kismet/GameplayStatics.h"
#include "Notification/GameNotificationManager.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

AInventorySystem::AInventorySystem(): ID(0), Quantity(0), DropQuantity(0), Image(nullptr), SlotsUsedMainInvetory(0),
                                      SlotsUsed(0),
                                      InventoryWidget(nullptr),
                                      InventoryWrapBox(nullptr),
                                      ImageBtnCloseInventory(nullptr),
                                      InventoryBorder(nullptr), CountSlotVariable(nullptr),
                                      GameNotificationManager(nullptr), ReapTheUndeadCharacter(nullptr),
                                      StockIndexSelected(0),
                                      TotalUsedSlots(0), HoveredWidget(nullptr)
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
	UInventoryDataItems* SaveActualAssetData = nullptr;
    UButton* Button = ButtonsSlots[ButtonIndex];
    FButtonStyle ButtonStyle = Button->GetStyle();

	static float LastClickTime = -1.0f;
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (LastClickTime >= 0.0f && (CurrentTime - LastClickTime < 0.4f))
	{
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
                	
                	DataAssets.Add(SaveActualAssetData);

                    GameNotificationManager->SetTextNotification(FString::Printf(TEXT("%s a bien été supprimé du raccourcie %d"), *SaveActualAssetData->Image->GetName(), ButtonIndex), FColor::Green);
					SaveInventoryToFile();
                	LoadInventoryFromFile();
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
	static float LastClickTimeMain = -1.0f;
	const float CurrentTimeMain = GetWorld()->GetTimeSeconds();

	if (LastClickTimeMain >= 0.0f && (CurrentTimeMain - LastClickTimeMain < 0.3f))
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		FVector SpawnPosition = PlayerCharacter->GetActorLocation();
		FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

		ADropItem* DropItemInstance;
		if (ReapTheUndeadCharacter->GetDropItem())
		{
			DropItemInstance = ReapTheUndeadCharacter->GetDropItem();
		}
		else
		{
			DropItemInstance = GetWorld()->SpawnActor<ADropItem>(DropItemClass, SpawnPosition, SpawnRotation);
		}
		if (!DropItemInstances.Find(DropItemInstance)) DropItemInstances.Add(DropItemInstance);
		
		for (int i = 0; i < DataAssets.Num(); i++)
		{
			if (ButtonIndex == DataAssets[i]->UsedSlotMainInventory)
			{
				if (DataAssets[i]->Quantity < 1) break;
				DataAssets[i]->Quantity--;
				UInventoryDataItems* FoundDropItem = nullptr;
				for (int j = 0; j < DropItemInstances.Num(); ++j)
				{
					if (DropItemInstances[j])
					{
						FoundDropItem = DropItemInstances[j]->GetItemDropAt(DataAssets[i]);
						if (FoundDropItem) break;
					}
				}
				if (!FoundDropItem)
				{
					DataAssets[i]->DropQuantity = 0;
				}
				DropItemInstance->AddItemToDrop(DataAssets[i]);
				UE_LOG(LogTemp, Warning, TEXT("Nom drop : %s"), *DataAssets[i]->GetName());
				GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Vous avez drop l'item suivant: %s"), *DataAssets[i]->Image->GetName()), FColor::Cyan);
				LoadInventory();
				SaveInventoryToFile();
				LoadInventoryFromFile();
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < DataAssets.Num(); i++)
		{
			if (ButtonIndex == DataAssets[i]->UsedSlotMainInventory)
			{
				StockIndexSelected = i;
				ItemSelected = true;
				GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Vous avez sélectionné l'item suivant: %s"), *DataAssets[i]->Image->GetName()), FColor::Cyan);
				break;
			}
		}
	}

	LastClickTimeMain = CurrentTimeMain;
}

void AInventorySystem::LoadInventory()
{
    if (!InventoryWrapBox) return;
    if (!ImagesButtonsDropInventoryClass) return;

	InventoryWrapBox->ClearChildren();
	ImagesButtonsInventory.Reset();

	// for (UWidget* Child : InventoryWrapBox->GetAllChildren())
	// {
	//     UButton* Button = Cast<UButton>(Child);
	//     if (Button)
	//     {
	//         Button->SetVisibility(ESlateVisibility::Collapsed);
	//     }
	// }

	int Index = 0;
	TotalUsedSlots = 0;

	DataAssets.Sort([](const UInventoryDataItems& A, const UInventoryDataItems& B) {
		return A.UsedSlotMainInventory < B.UsedSlotMainInventory;
	});
	
	for (UInventoryDataItems* Data : DataAssets)
	{
	    /*if (Index < ImagesButtonsInventory.Num())
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
	        TotalUsedSlots++;
	    }*/
		USlotButtonInventory* NewButton = NewObject<USlotButtonInventory>(this, ImagesButtonsDropInventoryClass);

		if (NewButton)
		{
			FButtonStyle ButtonStyle = NewButton->GetStyle();

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

					NewButton->SetStyle(ButtonStyle);
					InventoryWrapBox->AddChildToWrapBox(NewButton);
					ImagesButtonsInventory.Add(NewButton);
				}
			}
		}
		Index++;
		TotalUsedSlots++;
	}

	SlotsUsedMainInvetory = TotalUsedSlots;
	CountSlot = FString::Printf(TEXT("%d/%d"), TotalUsedSlots, MaxSlot);

	AttachOnClickedEvent();
	AttachHoveredEvent();
	AttachUnHoveredEvent();
}

void AInventorySystem::AddItem(UInventoryDataItems* ItemData, int Amount)
{
    if (DataAssets.Find(ItemData) && ItemData->Quantity > 0)
    {
        ItemData->Quantity += Amount;
    	ItemData->InInventory = true;

    	UE_LOG(LogTemp, Warning, TEXT("+1"));
    	SaveInventoryToFile();
    }
    else
    {
    	LoadInventoryFromFileWithItem(ItemData, Amount);
    	UE_LOG(LogTemp, Warning, TEXT("new"));
    }
	
	LoadInventoryFromFile();
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

	LoadInventoryFromFile();
	SaveInventoryToFile();
}

void AInventorySystem::InteractInventory()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!InventoryWidget && !InventoryBorder && !PlayerController && !ImageBtnCloseInventory && !HoveredWidget) return;

	if (IsOpen)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetIgnoreMoveInput(false);
		InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Hidden);
		CountSlotVariable->SetVisibility(ESlateVisibility::Hidden);
		HoveredWidget->RemoveFromParent();

		StockIndexSelected = -1;
		ItemSelected = false;
	}
	else
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetIgnoreMoveInput(true);
		InventoryBorder->SetVisibility(ESlateVisibility::Visible);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Visible);
		CountSlotVariable->SetVisibility(ESlateVisibility::Visible);
		HoveredWidget->AddToViewport();
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

void AInventorySystem::ShowItemsInInventorySlot2(int Index, UInventoryDataItems* DataAsset)
{
	InventorySlots.Add(Index, DataAsset->ItemClass);
	DataAsset->InMainInventory = false;
	DataAsset->UsedSlot = Index;
	DataAsset->UsedSlotMainInventory = -1;

	FButtonStyle ButtonStyle = ButtonsSlots[Index]->GetStyle();
			
	ButtonStyle.Normal.SetResourceObject(DataAsset->Image);
	ButtonStyle.Hovered.SetResourceObject(DataAsset->Image);
	ButtonStyle.Pressed.SetResourceObject(DataAsset->Image);
			
	FVector2D ImageSize(64.f, 64.f);
	ButtonStyle.Normal.SetImageSize(ImageSize);
	ButtonStyle.Hovered.SetImageSize(ImageSize);
	ButtonStyle.Pressed.SetImageSize(ImageSize);

	ButtonsSlots[Index]->SetStyle(ButtonStyle);
	ItemSelected = false;
}

UClass* AInventorySystem::FoundClassInSlot(int32 Index)
{
	if (UClass** ItemClass = InventorySlots.Find(Index))
	{
		return *ItemClass;
	}

	return nullptr;
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
			JsonObject->SetStringField(TEXT("Name"), Item->Name);
			JsonObject->SetNumberField(TEXT("ID"), Item->ID);
			JsonObject->SetNumberField(TEXT("Quantity"), Item->Quantity);
			JsonObject->SetNumberField(TEXT("UsedSlotMainInventory"), Item->UsedSlotMainInventory);
			JsonObject->SetNumberField(TEXT("UsedSlot"), Item->UsedSlot);
			JsonObject->SetBoolField(TEXT("InMainInventory"), Item->InMainInventory);
			JsonObject->SetBoolField(TEXT("InInventory"), Item->InInventory);
			JsonObject->SetNumberField(TEXT("StockIDInSlot"), Item->StockIDInSlot);
			JsonObject->SetNumberField(TEXT("DropQuantity"), Item->DropQuantity);

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
    int Index = 0;

    if (FFileHelper::LoadFileToString(FileContent, *SaveFilePath))
    {
        TSharedPtr<FJsonObject> RootObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContent);

        if (FJsonSerializer::Deserialize(JsonReader, RootObject) && RootObject.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>& JsonItems = RootObject->GetArrayField(TEXT("Items"));

            if (JsonItems.Num() > 0)
            {
                TArray<UInventoryDataItems*> ValidItems;

                for (const TSharedPtr<FJsonValue>& JsonValue : JsonItems)
                {
                    TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
                    int32 ItemID = JsonObject->GetNumberField(TEXT("ID"));

                    UInventoryDataItems* FoundAllDataAsset = nullptr;

                    for (UInventoryDataItems* DataAsset : AllDataAssets)
                    {
                        if (DataAsset && DataAsset->ID == ItemID)
                        {
                            DataAsset->UsedSlotMainInventory = Index;
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
                        FoundAllDataAsset->DropQuantity = JsonObject->GetNumberField(TEXT("DropQuantity"));

                        if (FoundAllDataAsset->Quantity > 0 && FoundAllDataAsset->InMainInventory)
                        {
                            ValidItems.Add(FoundAllDataAsset);
                        }

                        if (!FoundAllDataAsset->InMainInventory)
                        {
                            ShowItemsInInventorySlot2(FoundAllDataAsset->UsedSlot, FoundAllDataAsset);
                        }
                    }

                    Index++;
                }

                DataAssets = ValidItems;

                int NewSlotIndex = 0;
                for (UInventoryDataItems* Item : DataAssets)
                {
                    if (Item)
                    {
                        Item->UsedSlotMainInventory = NewSlotIndex;
                        NewSlotIndex++;
                    }
                }

                LoadInventory();
                EnsureUniqueAndCompactSlots();
            }
        }
    }
}

void AInventorySystem::EnsureUniqueAndCompactSlots()
{
	TSet<int32> UsedSlots;
	TArray<UInventoryDataItems*> ItemsToFix;
	TArray<int32> AvailableSlots;

	for (UInventoryDataItems* DataAsset : DataAssets)
	{
		if (!UsedSlots.Contains(DataAsset->UsedSlotMainInventory))
		{
			UsedSlots.Add(DataAsset->UsedSlotMainInventory);
		}
		else
		{
			ItemsToFix.Add(DataAsset);
		}
	}

	int32 MaxSlotTemp = UsedSlots.Num() + ItemsToFix.Num();
	for (int32 i = 0; i < MaxSlotTemp; i++)
	{
		if (!UsedSlots.Contains(i))
		{
			AvailableSlots.Add(i);
		}
	}

	int32 AvailableIndex = 0;
	for (UInventoryDataItems* DataAsset : ItemsToFix)
	{
		if (AvailableIndex < AvailableSlots.Num())
		{
			DataAsset->UsedSlotMainInventory = AvailableSlots[AvailableIndex];
			UsedSlots.Add(AvailableSlots[AvailableIndex]);
			AvailableIndex++;
		}
	}
}

void AInventorySystem::LoadInventoryFromFileWithItem(UInventoryDataItems* ItemData, int Amount)
{
    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *SaveFilePath))
    {
        TSharedPtr<FJsonObject> RootObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContent);

        if (FJsonSerializer::Deserialize(JsonReader, RootObject) && RootObject.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>& JsonItems = RootObject->GetArrayField(TEXT("Items"));

            bool bItemExists = false;

            for (const TSharedPtr<FJsonValue>& JsonValue : JsonItems)
            {
                TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
                int32 ItemID = JsonObject->GetNumberField(TEXT("ID"));

                UInventoryDataItems* FoundAllDataAsset = nullptr;

                for (UInventoryDataItems* DataAsset : DataAssets)
                {
                    if (DataAsset && DataAsset->ID == ItemID)
                    {
                        FoundAllDataAsset = DataAsset;
                        break;
                    }
                }

                if (FoundAllDataAsset)
                {
                    if (FoundAllDataAsset->ID == ItemData->ID)
                    {
                        FoundAllDataAsset->Quantity += Amount;
                        bItemExists = true;
                    }
                }
            }

            if (!bItemExists)
            {
                ItemData->Quantity = Amount;
                ItemData->UsedSlotMainInventory = DataAssets.Num();
                ItemData->InMainInventory = true;
                ItemData->InInventory = true;
            	
                DataAssets.Add(ItemData);
            }
            SaveInventoryToFile();
        }
    }
    SaveInventoryToFile();
}

void AInventorySystem::RemoveDropInstance(ADropItem* InstanceData)
{
	if (DropItemInstances.Contains(InstanceData))
	{
		DropItemInstances.Remove(InstanceData);
	}
}

void AInventorySystem::AttachHoveredEvent()
{
	if (UFunction* BindingAllItemsHoveredFunction = InventoryWidget->FindFunction(TEXT("BindingAllItemsHovered")))
	{
		InventoryWidget->ProcessEvent(BindingAllItemsHoveredFunction, nullptr);
	}
}

void AInventorySystem::AttachUnHoveredEvent()
{
	if (UFunction* BindingAllItemsUnHoveredFunction = InventoryWidget->FindFunction(TEXT("BindingAllItemsUnHovered")))
	{
		InventoryWidget->ProcessEvent(BindingAllItemsUnHoveredFunction, nullptr);
	}
}

void AInventorySystem::TriggerHoveredButtonDelegate(int ButtonIDVar)
{
	if (!HoveredWidget) return;

	for (int i = 0; i < DataAssets.Num(); i++)
	{
		if (i == ButtonIDVar)
		{
			QuantityText = FString::Printf(TEXT("Nom : %s\nQuantity : %d"), *DataAssets[i]->Name, DataAssets[i]->Quantity);
			HoveredWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			break;
		}
	}
}

void AInventorySystem::TriggerUnHoveredButtonDelegate(int ButtonIDVar)
{
	if (!HoveredWidget) return;

	HoveredWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AInventorySystem::AttachOnClickedEvent()
{
	if (UFunction* BindingAllItemsOnClickedFunction = InventoryWidget->FindFunction(TEXT("BindingAllItemsOnClicked")))
	{
		InventoryWidget->ProcessEvent(BindingAllItemsOnClickedFunction, nullptr);
	}
}