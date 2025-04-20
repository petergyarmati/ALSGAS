// Copyright mrgyarmati

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGXEffectActor.generated.h"

class UBoxComponent;

UCLASS()
class ALSABILITYCORE_API AAGXEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAGXEffectActor();



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

};
