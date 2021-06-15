// Copyright Soren Gilbertson


#include "ColorCurveKey.h"
#include "Curves/CurveLinearColor.h"
#include "Components/CanvasPanelSlot.h"


void UColorCurveKey::SetNewTime(float NewTime)
{
	Time = NewTime;
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
	CanvasSlot->SetPosition(FVector2D(FMath::Lerp(0, 512, Time), 0));
}

void UColorCurveKey::RemoveKey()
{
	for (int8 i = 0; i < 4; i++)
	{
		Gradient->FloatCurves[i].DeleteKey(Handle);
	}
}
