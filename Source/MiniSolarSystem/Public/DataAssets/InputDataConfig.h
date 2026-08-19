// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataConfig.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UInputDataConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SwitchPerspective;
};


UCLASS()
class MINISOLARSYSTEM_API UInputDataConfig_Celestial : public UInputDataConfig
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Move;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Look;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ChangeSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* IgnoreGravity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* FocusPlanet;
};


UCLASS()
class MINISOLARSYSTEM_API UInputDataConfig_Overview : public UInputDataConfig
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Move;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Look;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ChangeSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Zoom;
};
