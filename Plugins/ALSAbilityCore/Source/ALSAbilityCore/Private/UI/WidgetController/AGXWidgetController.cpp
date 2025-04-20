// Copyright mrgyarmati


#include "UI/WidgetController/AGXWidgetController.h"

void UAGXWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& widgetControllerParams)
{
	PlayerController = widgetControllerParams.PlayerController;
	PlayerState = widgetControllerParams.PlayerState;
	AbilitySystemComponent = widgetControllerParams.AbilitySystemComponent;
	AttributeSet = widgetControllerParams.AttributeSet;
}

void UAGXWidgetController::BroadcastInitialValues()
{
}

void UAGXWidgetController::BindCallbacksToDependencies()
{
}
