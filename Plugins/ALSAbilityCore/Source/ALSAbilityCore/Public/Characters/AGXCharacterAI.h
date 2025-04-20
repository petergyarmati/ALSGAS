// Copyright mrgyarmati

#pragma once

#include "CoreMinimal.h"
#include "AGXCharacterBase.h"
#include "AGXCharacterAI.generated.h"

UCLASS()
class ALSABILITYCORE_API AAGXCharacterAI : public AAGXCharacterBase
{
	GENERATED_BODY()

public:
	AAGXCharacterAI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
