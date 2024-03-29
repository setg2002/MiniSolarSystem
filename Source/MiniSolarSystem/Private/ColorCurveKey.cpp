// Copyright Soren Gilbertson


#include "ColorCurveKey.h"
#include "Curves/CurveLinearColor.h"
#include "Components/CanvasPanelSlot.h"

void UColorCurveKey::NativeConstruct()
{
	Super::NativeConstruct();

	bHasScriptImplementedTick = false;
}

void UColorCurveKey::SetNewTime(float NewTime, bool bUpdate)
{
	Time = NewTime;
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
	CanvasSlot->SetPosition(FVector2D(FMath::Lerp(0, 512, Time), 0));
	
	for (int8 i = 0; i < 4; i++)
	{
		Gradient->FloatCurves[i].SetKeyTime(Handle, NewTime);
	}
	if (bUpdate)
		OnKeyUpdated.ExecuteIfBound();
}

void UColorCurveKey::SetNewColor(FLinearColor NewColor, bool bUpdate)
{
	Color = NewColor;

	for (int8 i = 0; i < 4; i++)
	{
		Gradient->FloatCurves[i].SetKeyValue(Handle, i == 0 ? Color.R : i == 1 ? Color.G : i == 2 ? Color.B : Color.A);
	}
	if (bUpdate)
		OnKeyUpdated.ExecuteIfBound();
}

void UColorCurveKey::RemoveKey()
{
	for (int8 i = 0; i < 4; i++)
	{
		if (Gradient->FloatCurves[i].GetIndexSafe(Handle) != INDEX_NONE)
		{
			Gradient->FloatCurves[i].DeleteKey(Handle);
		}
	}
}
