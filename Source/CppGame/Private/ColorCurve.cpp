// Copyright Soren Gilbertson


#include "ColorCurve.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Curves/CurveLinearColor.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "ColorCurveKey.h"




void UColorCurve::NativeConstruct()
{
	Super::NativeConstruct();

	for (auto& Key : Gradient->FloatCurves[0].Keys)
	{
		AddKey(Key.Time, Gradient->GetLinearColorValue(Key.Time), false);
	}
	// Bind delegates here.
}

TSharedRef<SWidget> UColorCurve::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	return Widget;
}

void UColorCurve::AddKey(float Time, FLinearColor Color = FLinearColor::White, bool Update = false)
{
	UColorCurveKey* NewKey = Cast<UColorCurveKey>(CreateWidget<UColorCurveKey>(CanvasPanel_Gradient, KeyClass, FName("Key_" + FString::SanitizeFloat(Time))));
	NewKey->SetVisibility(ESlateVisibility::Visible);
	CanvasPanel_Gradient->AddChildToCanvas(NewKey);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewKey->Slot);
	CanvasSlot->SetAnchors(FAnchors(0));
	CanvasSlot->SetAlignment(FVector2D::ZeroVector);
	CanvasSlot->SetSize(FVector2D::ZeroVector);
	NewKey->Color = Color;
	NewKey->SetNewTime(Time);
	NewKey->Gradient = Gradient;
	if (Update)
	{
		FKeyHandle NewKeyHandle = FKeyHandle();
		for (int8 i = 0; i < 4; i++)
		{
			//TODO Is this the best way to write this line?
			Gradient->FloatCurves[i].AddKey(Time, i == 0 ? Color.R : i == 1 ? Color.G : i == 2 ? Color.B :Color.A, false, NewKeyHandle);
		}
		NewKey->Handle = NewKeyHandle;
	}

}
