// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColorCurve.generated.h"

/**
 * 
 */
USTRUCT()
struct FKeyInfo
{
	GENERATED_BODY()

	int8 RGB;
	float Time;
	float Val;

	FKeyInfo() // Default Constructor
	{
		RGB = 0;
		Time = 0.f;
		Val = 0.f;
	}

	FKeyInfo(int8 rgb, float time, float val) // Full Constructor
		:RGB(rgb), Time(time), Val(val)
	{

	}

	FORCEINLINE bool operator==(const FKeyInfo& other) const
	{
		return other.RGB == RGB && other.Time == Time && other.Val == Val;
	}
};


UCLASS()
class MINISOLARSYSTEM_API UColorCurve : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ColorCurveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> KeyClass;

	// Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Gradient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Gradient;

	UFUNCTION(BlueprintCallable)
	class UColorCurveKey* AddKey(float Time, FLinearColor Color = FLinearColor::White);

	UFUNCTION(BlueprintCallable)
	void UpdateGradient();

	UFUNCTION(BlueprintCallable)
	FName GetName();

	UFUNCTION(BlueprintCallable)
	bool SetName(FString NewName);

protected:
	FLinearColor MakeColor(TArray<TTuple<int8, float>> ColorValues);

	void ConstructKey(TArray<FKeyInfo> KeysInfo);
};
