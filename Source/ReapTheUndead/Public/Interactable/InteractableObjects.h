#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObjects.generated.h"

class UBoxComponent;

UCLASS()
class REAPTHEUNDEAD_API AInteractableObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableObjects();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex);
private:	
	bool isPlayerTrigger = false;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Objectmesh;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	APawn* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Overlay", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* MaterialOverlay;
};
