// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColorCurveKey.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UColorCurveKey : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;

	UCurveLinearColor* Gradient;
	FKeyHandle Handle;

	UFUNCTION(BlueprintCallable)
	void SetNewTime(float NewTime);

	UFUNCTION(BlueprintCallable)
	void RemoveKey();
};
