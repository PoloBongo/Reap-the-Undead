#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LumberJack.generated.h"

class AResourcesManager;

UCLASS()
class REAPTHEUNDEAD_API ALumberJack : public AActor
{
	GENERATED_BODY()
	
public:	
	ALumberJack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	AResourcesManager* ResourcesManager;
	
};
