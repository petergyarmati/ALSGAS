// Copyright mrgyarmati

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGXUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALSABILITYCORE_API UAGXUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
