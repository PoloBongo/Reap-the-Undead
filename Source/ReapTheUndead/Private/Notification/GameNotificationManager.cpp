#include "Notification/GameNotificationManager.h"

#include "Blueprint/UserWidget.h"

AGameNotificationManager* AGameNotificationManager::Instance = nullptr;

AGameNotificationManager* AGameNotificationManager::GetInstance(UWorld* World)
{
	if (!Instance)
	{
		if (World)
		{
			Instance = World->SpawnActor<AGameNotificationManager>(AGameNotificationManager::StaticClass());
		}
	}

	return Instance;
}

AGameNotificationManager::AGameNotificationManager(): NotificationWidget(nullptr), NotificationDuration(0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameNotificationManager::SetTextNotification(FString NewTextNotification, FColor Color)
{
	FNotification NewNotification;
	NewNotification.Message = NewTextNotification;
	NewNotification.Color = Color;
	ShowNotification(NewNotification);
}

void AGameNotificationManager::BeginPlay()
{
	Super::BeginPlay();
}

FString AGameNotificationManager::GetTextNotification()
{
	return TextNotification;
}

void AGameNotificationManager::ShowNotification(const FNotification& NewNotification)
{
	if (!IsValid(this) || !IsValid(NotificationWidget) || !GetWorld())
	{
		return;
	}

	TextNotification = NewNotification.Message;
	NotificationWidget->SetColorAndOpacity(NewNotification.Color);
	NotificationQueue.Enqueue(NewNotification);

	if (!GetWorld()->GetTimerManager().IsTimerActive(NotificationHandle))
	{
		ProcessNextNotification();
	}
}

void AGameNotificationManager::ProcessNextNotification()
{
	FNotification NextNotification;

	// check les files d'attentes
	if (NotificationQueue.Dequeue(NextNotification))
	{
		if (NotificationWidget)
		{
			NotificationWidget->SetColorAndOpacity(NextNotification.Color);
			NotificationWidget->AddToViewport();
		}

		GetWorld()->GetTimerManager().SetTimer(NotificationHandle, this, &AGameNotificationManager::HiddeNotification, NotificationDuration, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(NotificationHandle);
	}
}

void AGameNotificationManager::HiddeNotification()
{
	if (NotificationWidget)
	{
		NotificationWidget->RemoveFromParent();
	}

	ProcessNextNotification();
}