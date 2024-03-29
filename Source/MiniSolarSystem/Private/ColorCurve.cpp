// Copyright Soren Gilbertson


#include "ColorCurve.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "ColorCurveFunctionLibrary.h"
#include "Curves/CurveLinearColor.h"
#include "Components/CanvasPanel.h"
#include "GasGiantColorSettings.h"
#include "Blueprint/WidgetTree.h"
#include "RingSystemComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "ColorCurveKey.h"
#include "GasGiant.h"
#include "Planet.h"



void UColorCurve::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<FKeyInfo> Keys;
	for (int8 i = 0; i < 4; i++)
	{
		for (auto It = Gradient->FloatCurves[i].GetKeyHandleIterator(); It; ++It)
		{
			float time = Gradient->FloatCurves[i].GetKeyTime(*It);
			float val = Gradient->FloatCurves[i].GetKeyValue(*It);
			Keys.AddUnique(FKeyInfo(i, time, val));
		}
	}	
	Keys.Sort([](const FKeyInfo& a, const FKeyInfo& b) { return a.Time > b.Time || (a.Time == b.Time && a.RGB < b.RGB); });

	//  The following loop is meant to add "missing keys" to the existing color curve. This is because in the unreal editor,
	// when creating a color curve, the editor only puts keys on the curve for colors that change to save space. However,
	// for the use of the widget we need a key to exist for every color regardless of if it changed. To do this we start with
	// the template. The template array contains a key for each color with the value and time 0. Looping through the array
	// Keys then, if we find a key we replace the time and value of the corresponding color. When we reach a new key, the
	// KeyToMake is constructed and is then reset to the template and the loop begins again.
	if (Keys.Num() > 0)
	{
		TArray<FKeyInfo> Template = { FKeyInfo(0, 0, 0), FKeyInfo(1, 0, 0), FKeyInfo(2, 0, 0), FKeyInfo(3, 0, 1) };
		TArray<FKeyInfo> KeyToMake = Template;
		KeyToMake.RemoveAt(Keys[0].RGB, 1);
		KeyToMake.EmplaceAt(Keys[0].RGB, Keys[0]);
		for (auto& Key : Keys)
		{
			if (KeyToMake.Num() == 0)
			{
				KeyToMake = Template;
				for (auto& Info : KeyToMake)
				{
					Info.Time = Key.Time;
				}
				KeyToMake.RemoveAt(Key.RGB, 1);
				KeyToMake.EmplaceAt(0, Key);
			}
			else if (Key.Time == KeyToMake[0].Time)
			{
				KeyToMake.RemoveAt(Key.RGB, 1);
				KeyToMake.EmplaceAt(Key.RGB, Key);
			}
			else
			{
				ConstructKey(KeyToMake);
				KeyToMake.Empty();
				KeyToMake = Template;
				for (auto& Info : KeyToMake)
				{
					Info.Time = Key.Time;
				}
				KeyToMake.RemoveAt(Key.RGB, 1);
				KeyToMake.EmplaceAt(0, Key);
			}
		}
		ConstructKey(KeyToMake);
	}

	UpdateGradient();
	// Bind delegates here.
}

TSharedRef<SWidget> UColorCurve::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	return Widget;
}

FLinearColor UColorCurve::MakeColor(TArray<TTuple<int8, float>> ColorValues)
{
	float Red = 0;
	float Green = 0;
	float Blue = 0;
	//float Alpha = 0;

	for (int8 i = 0; i < ColorValues.Num(); i++)
	{
		if (ColorValues[i].Key == 0)
			Red = ColorValues[i].Value;
		else if (ColorValues[i].Key == 1)
			Green = ColorValues[i].Value;
		else if (ColorValues[i].Key == 2)
			Blue = ColorValues[i].Value;
		//else if (ColorValues[i].Key == 3)
		//	Alpha = ColorValues[i].Value;
	}
	return FLinearColor(Red, Green, Blue, 1);
}

void UColorCurve::ConstructKey(TArray<FKeyInfo> KeysInfo)
{
	float Time = KeysInfo[0].Time;

	TArray<TTuple<int8, float>> RGBValues;
	for (int8 i = 0; i < KeysInfo.Num(); i++)
	{
		RGBValues.Add(TTuple<int8, float>(KeysInfo[i].RGB, KeysInfo[i].Val));
	}
	FLinearColor Color = MakeColor(RGBValues);

	// Create the widget
	UColorCurveKey* NewKey = Cast<UColorCurveKey>(CreateWidget<UColorCurveKey>(CanvasPanel_Gradient, KeyClass, FName("Key_" + FString::SanitizeFloat(Time))));
	NewKey->OnKeyUpdated.BindUFunction(this, "UpdateGradient");
	NewKey->SetVisibility(ESlateVisibility::Visible);
	CanvasPanel_Gradient->AddChildToCanvas(NewKey);
	
	// Setup layout
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewKey->Slot);
	CanvasSlot->SetAnchors(FAnchors(0));
	CanvasSlot->SetAlignment(FVector2D::ZeroVector);
	CanvasSlot->SetSize(FVector2D::ZeroVector);
	CanvasSlot->SetPosition(FVector2D(FMath::Lerp(0, 512, Time), 0));
	
	// Set variables
	NewKey->Color = Color;
	NewKey->Time = Time;
	NewKey->Gradient = Gradient;
	TArray<FKeyHandle> Handles;
	Handles.SetNum(4);
	FKeyHandle NewKeyHandle = FKeyHandle();
	for (int8 i = 0; i < 4; i++)
	{
		Handles[i] = Gradient->FloatCurves[i].UpdateOrAddKey(Time, KeysInfo[i].Val);
		Gradient->FloatCurves[i].DeleteKey(Handles[i]);
		Gradient->FloatCurves[i].AddKey(Time, i == 0 ? Color.R : i == 1 ? Color.G : i == 2 ? Color.B : Color.A, false, NewKeyHandle);
	}
	NewKey->Handle = NewKeyHandle;
}

UColorCurveKey* UColorCurve::AddKey(float Time, FLinearColor Color)
{
	// Create the widget
	UColorCurveKey* NewKey = Cast<UColorCurveKey>(CreateWidget<UColorCurveKey>(CanvasPanel_Gradient, KeyClass, FName("Key_" + FString::SanitizeFloat(Time))));
	NewKey->OnKeyUpdated.BindUFunction(this, "UpdateGradient");
	NewKey->SetVisibility(ESlateVisibility::Visible);
	CanvasPanel_Gradient->AddChildToCanvas(NewKey);

	// Setup layout
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewKey->Slot);
	CanvasSlot->SetAnchors(FAnchors(0));
	CanvasSlot->SetAlignment(FVector2D::ZeroVector);
	CanvasSlot->SetSize(FVector2D::ZeroVector);
	CanvasSlot->SetPosition(FVector2D(FMath::Lerp(0, 512, Time), 0));

	// Set variables
	NewKey->Color = Color;
	NewKey->Time = Time;
	NewKey->Gradient = Gradient;

	FKeyHandle NewKeyHandle = FKeyHandle();
	for (int8 i = 0; i < 4; i++)
	{
		Gradient->FloatCurves[i].AddKey(Time, i == 0 ? Color.R : i == 1 ? Color.G : i == 2 ? Color.B : Color.A, false, NewKeyHandle);
	}
	NewKey->Handle = NewKeyHandle;

	return NewKey;
}

void UColorCurve::UpdateGradient()
{
	Gradient->OnGradientUpdated.Broadcast();
}

FName UColorCurve::GetName()
{
	Gradient->GetName(ColorCurveName);
	return (FName)ColorCurveName;
}

bool UColorCurve::SetName(FString NewName)
{
	return Gradient->Rename(*NewName);
}
