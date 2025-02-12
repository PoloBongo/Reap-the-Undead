#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObjects.generated.h"

class UWidgetComponent;
struct FInputActionValue;
class UBoxComponent;
class UUserWidget;

DECLARE_DELEGATE(FOnInteractDelegate);

UCLASS()
class REAPTHEUNDEAD_API AInteractableObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableObjects();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	virtual void Tick(float DeltaTime) override;

	virtual void InteractFunction();
protected:
	virtual void BeginPlay() override;

	virtual void InteractObject();
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Overlay", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* MaterialOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* UIUserInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* UIHarvest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* UIUserInteractComponent;

	APawn* PlayerController;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Objectmesh;
private:
	bool isPlayerTrigger = false;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
};
