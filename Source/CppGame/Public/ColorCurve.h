// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColorCurve.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UColorCurve : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> KeyClass;

	// Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Gradient;

	UFUNCTION(BlueprintCallable)
	void AddKey(float Time, FLinearColor Color);
};
