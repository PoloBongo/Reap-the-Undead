#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameNotificationManager.generated.h"

USTRUCT(BlueprintType)
struct FNotification
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Message; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor Color;

	FNotification() : Message(TEXT("Default Message")), Color(FLinearColor::White) {}
};

UCLASS()
class REAPTHEUNDEAD_API AGameNotificationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameNotificationManager();

static AGameNotificationManager* GetInstance(UWorld* World);

	void SetTextNotification(FString NewNotification, FColor Color);
	void ShowNotification(const FNotification& NewNotification);
	FString GetTextNotification();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	FString TextNotification;
	
	FTimerHandle NotificationHandle;
	FTimerManager NotificationManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UUserWidget* NotificationWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	float NotificationDuration;

	TQueue<FNotification> NotificationQueue;
	
	void HiddeNotification();
	void ProcessNextNotification();

	static AGameNotificationManager* Instance;
};
