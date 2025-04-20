// Copyright mrgyarmati


#include "UI/WidgetController/AGXOverlayWidgetController.h"

#include "AbilitySystem/AGXAttributeSet.h"

void UAGXOverlayWidgetController::BroadcastInitialValues()
{
	const UAGXAttributeSet* AgxAttributeSet = Cast<UAGXAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AgxAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AgxAttributeSet->GetMaxHealth());
	OnStaminaChanged.Broadcast(AgxAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(AgxAttributeSet->GetMaxStamina());

	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AgxAttributeSet->GetHealthAttribute());
}

void UAGXOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAGXAttributeSet* AgxAttributeSet = Cast<UAGXAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AgxAttributeSet->GetHealthAttribute()).AddUObject(this, &UAGXOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AgxAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UAGXOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AgxAttributeSet->GetStaminaAttribute()).AddUObject(this, &UAGXOverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AgxAttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &UAGXOverlayWidgetController::MaxStaminaChanged);
}

void UAGXOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UAGXOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UAGXOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UAGXOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
