// Copyright mrgyarmati


#include "UI/Widgets/AGXUserWidget.h"

void UAGXUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
