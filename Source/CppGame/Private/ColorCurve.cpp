// Copyright Soren Gilbertson


#include "ColorCurve.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"




void UColorCurve::NativeConstruct()
{
	Super::NativeConstruct();

	//UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	//Button_Main = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Button"));
	//Button_Main->OnClicked.AddDynamic(this, &MyUserWidget::OnButtonClicked); // Example click binding.
	//RootWidget->AddChild(Button_Main);

	// Bind delegates here.
}

TSharedRef<SWidget> UColorCurve::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	/*if (WidgetTree)
	{
		SNew(SButton).Text(FText::FromString("Example"));
	}*/
	return Widget;
}

void UColorCurve::AddKey(float Time, FLinearColor Color = FLinearColor::White)
{
	//auto AClass = KeyClass.Get();
	//CreateWidget<AClass>(this);
}
