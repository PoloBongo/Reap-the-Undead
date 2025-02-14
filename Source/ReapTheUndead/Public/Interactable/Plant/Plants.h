#pragma once

#include "CoreMinimal.h"
#include "Planting/PlantManager.h"
#include "Plants.generated.h"

class AGameNotificationManager;
class UInventoryDataItems;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlantOnTextChanged, const FString&, NewText);
UCLASS()
class REAPTHEUNDEAD_API APlants : public APlantManager
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Text")
	void ChangeText(const FString& NewText);
	void SetCanHarvest(bool Harvest);
	virtual void Destroyed() override;

protected:
	virtual void InteractObject() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	bool CanDeclenchAnim = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	bool IsAlreadyPlanted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	bool CanHarvest = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	FString StringPlant;

private:
	APlantingSystem* PlantingSystem;
	UStaticMeshComponent* ActualEvolveMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI Interact", meta=(AllowPrivateAccess="true"))
	UInventoryDataItems* DataItems;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlantOnTextChanged OnTextChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	float DelayResetAnimation = 3.f;
	
	FTimerHandle PlantingAnimTimerHandle;
	void ResetAnimation();
};
