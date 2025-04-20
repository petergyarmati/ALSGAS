// Copyright mrgyarmati

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AGXHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UAGXOverlayWidgetController;
class UAGXUserWidget;
/**
 * 
 */
UCLASS()
class ALSABILITYCORE_API AAGXHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAGXUserWidget> OverlayWidget;

	UAGXOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGXUserWidget> OverlayWidgetClass;

	UPROPERTY()
	UAGXOverlayWidgetController* OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGXOverlayWidgetController> OverlayWidgetControllerClass;

protected:
	virtual void BeginPlay() override;
};
