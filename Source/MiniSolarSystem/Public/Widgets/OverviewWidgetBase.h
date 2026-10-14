// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverviewWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverviewSidebarToggled, bool, bOpen);

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UOverviewWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnOverviewSidebarToggled OnOverviewSidebarToggled;
};
