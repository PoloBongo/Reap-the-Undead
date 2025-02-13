#include "Interactable/NPC/InteractableNPC.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Interactable/NPC/DataCraftNPC.h"
#include "Inventory/InventorySystem.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "Kismet/GameplayStatics.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

void AInteractableNPC::BeginPlay()
{
	Super::BeginPlay();
	UIInteractableNPC->AddToViewport();

	if (!SendButton) return;
	SendButton->OnClicked.AddDynamic(this, &AInteractableNPC::CraftingCheck);
	if (!ArrowLeft) return;
	ArrowLeft->OnClicked.AddDynamic(this, &AInteractableNPC::GoLeft);
	if (!ArrowRight) return;
	ArrowRight->OnClicked.AddDynamic(this, &AInteractableNPC::GoRight);

	MaxIndexCraft = CraftAssets.Num() - 1;
}

void AInteractableNPC::InteractObject()
{
	Super::InteractObject();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("interactable"));
	
	UIInteractableNPC->AddToViewport();

	if (Sound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, this->GetActorLocation(), GetActorRotation());
	
	LoadCraft();
	SetInputModeNPC();
	
	IsOpen = !IsOpen;
}

void AInteractableNPC::SetInputModeNPC() const
{
	APlayerController* PlayerControllerVar = GetWorld()->GetFirstPlayerController();
	if (!UIInteractableNPC && !PlayerControllerVar && !CraftBorder) return;

	if (IsOpen)
	{
		PlayerControllerVar->SetInputMode(FInputModeGameOnly());
		PlayerControllerVar->SetIgnoreMoveInput(false);
		CraftBorder->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PlayerControllerVar->SetInputMode(FInputModeGameAndUI());
		PlayerControllerVar->SetIgnoreMoveInput(true);
		CraftBorder->SetVisibility(ESlateVisibility::Visible);
	}
	PlayerControllerVar->SetShowMouseCursor(!IsOpen);
}

void AInteractableNPC::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerController != OtherActor) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OUAIS MANUELO"));

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(this);
	}
}

void AInteractableNPC::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DEGAGE BATARD %d"));

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(nullptr);
	}
}

void AInteractableNPC::CraftingCheck()
{
	int TotalCheck = 0;
	for (int i = 0; i < StocksPlacementItems.Num(); i++)
	{
		if (StocksPlacementItems[i] == CraftAssets[IndexCraft]->Datas[i])
		{
			TotalCheck++;
		}
	}

	if (TotalCheck == CraftAssets[IndexCraft]->Datas.Num())
	{
		for (int i = 0; i < StocksPlacementItems.Num(); i++)
		{
			InventorySystem->RemoveItem(CraftAssets[IndexCraft]->Datas[i]);
		}
		InventorySystem->AddItem(CraftAssets[IndexCraft]->RewardItem);
	}
}

void AInteractableNPC::LoadCraft()
{
	StocksPlacementItems.Reset();
	WrapBox->ClearChildren();
	if (InventorySystem->GetDataAssets().Num() > 0 && CraftAssets.Num() > 0)
	{
		for (int i = 0; i < CraftAssets[IndexCraft]->Datas.Num(); i++)
		{
			for (int j = 0; j < InventorySystem->GetDataAssets().Num(); j++)
			{
				if (InventorySystem->GetDataAssets()[j]->Image == CraftAssets[IndexCraft]->Datas[i]->Image)
				{
					if (InventorySystem->GetDataAssets()[j]->Quantity > 0)
					{
						UButton* NewButton = NewObject<UButton>(WrapBox);
						FButtonStyle ButtonStyle = NewButton->GetStyle();
        
						ButtonStyle.Normal.SetResourceObject(CraftAssets[IndexCraft]->Datas[i]->Image);
						ButtonStyle.Hovered.SetResourceObject(CraftAssets[IndexCraft]->Datas[i]->Image);
						ButtonStyle.Pressed.SetResourceObject(CraftAssets[IndexCraft]->Datas[i]->Image);

						ButtonStyle.Normal.DrawAs = ESlateBrushDrawType::Box;
						ButtonStyle.Hovered.DrawAs = ESlateBrushDrawType::Box;
						ButtonStyle.Pressed.DrawAs = ESlateBrushDrawType::Box;
						
						FVector2D ImageSize(80.f, 275.f);
						ButtonStyle.Normal.SetImageSize(ImageSize);
						ButtonStyle.Hovered.SetImageSize(ImageSize);
						ButtonStyle.Pressed.SetImageSize(ImageSize);
        
						NewButton->SetStyle(ButtonStyle);
						WrapBox->AddChildToWrapBox(NewButton);
						StocksPlacementItems.Add(InventorySystem->GetDataAssets()[j]);
					}
					else
					{
						// pas assez d'item
					}
				}
			}
		}
		CraftingName = TEXT("Crafter ") + CraftAssets[IndexCraft]->RewardItem->GetName();
	}
}

void AInteractableNPC::GoLeft()
{
	if (IndexCraft > 0)
	{
		IndexCraft--;
		LoadCraft();
	}
}

void AInteractableNPC::GoRight()
{
	if (IndexCraft < MaxIndexCraft)
	{
		IndexCraft++;
		LoadCraft();
	}
}