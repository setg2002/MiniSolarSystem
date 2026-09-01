// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColorCurveKey.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UColorCurveKey : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FCurveKeyUpdated)

public:
	virtual void NativeConstruct() override;

	FCurveKeyUpdated OnKeyUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConstructed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;

	UCurveLinearColor* Gradient;
	FKeyHandle Handle;

	UFUNCTION(BlueprintCallable)
	void SetNewTime(float NewTime, bool bUpdate = true);

	UFUNCTION(BlueprintCallable)
	void SetNewColor(FLinearColor NewColor, bool bUpdate = true);

	UFUNCTION(BlueprintCallable)
	void RemoveKey();
};
