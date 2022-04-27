// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Field.h"
#include "Logging/LogVerbosity.h"
#include "Components/SplineComponent.h"
#include "Toolbox.generated.h"

//Incarna Log
DECLARE_LOG_CATEGORY_EXTERN(LogIncarnaEdit, VeryVerbose, VeryVerbose);
DECLARE_LOG_CATEGORY_EXTERN(LogIncarnaDev, Verbose, Verbose);
DECLARE_LOG_CATEGORY_EXTERN(LogIncarnaShip, Log, Log);

#define IncarnaEnumToString(EnumClassName, ValueOfEnum) UToolbox::GetEnumValueAsString<EnumClassName>(FString(TEXT(#EnumClassName)), (ValueOfEnum))

UENUM(BlueprintType)
enum ELogVerbosityType
{
	EFatal				UMETA(DisplayName = "Fatal"),
	EError				UMETA(DisplayName = "Error"),
	EWarning			UMETA(DisplayName = "Warning"),
	EDisplay			UMETA(DisplayName = "Display"),
	ELog				UMETA(DisplayName = "Log"),
	EVerbose			UMETA(DisplayName = "Verbose"),
	EVeryVerbose		UMETA(DisplayName = "VeryVerbose")
};

UCLASS()
class IAHIDEANDSEEK_API UToolbox : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Equivalent to Print String AND UE_LOG
	UFUNCTION(BlueprintCallable, Category = "Toolbox", meta = (CallableWithoutWorldContext, AdvancedDisplay = "1"))
	static void PrintLog(FString InString = "Hello", bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);

	static void PrintLog(const char* InString = "Hello", bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);
	
	static void PrintLog(float InString = 0.0f, bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);
	
	static void PrintLog(int InString = 0, bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);
	
	static void PrintLog(bool InString = true, bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);
	
	static void PrintLog(FVector InString = FVector::ZeroVector, bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);
	
	static void PrintLog(FRotator InString = FRotator::ZeroRotator, bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);
	
	static void PrintLog(UObject* InString, bool PrintToScreen = true, bool PrintToLog = true, ELogVerbosityType VerbosityLevel = EDisplay, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.0f);

	// Called to know in which Configuration we are
	// EDITOR - EDITOR_PLAY - DEVELOPMENT - SHIPPING
	UFUNCTION(BlueprintCallable, Category = "Toolbox")
	static FString GetBuildConfiguration();

	// Copy the Index Point of the SplineComponent
	UFUNCTION(BlueprintCallable, Category = "Toolbox")
	static FSplinePoint CopySplinePoint(USplineComponent* SplineComponent, int IndexPoint, bool IsLocal = true);

	// Make a Color in Editor simplier
	UFUNCTION(BlueprintPure, Category = "Toolbox", meta = (CompactNodeTitle = "Color"))
	static FColor MakeSimpleColor(FLinearColor SimpleColor = FLinearColor::White);

	template<class T>
	static FORCEINLINE void ShuffleArray(const TArray<T>& TargetArray)
	{

		int32 LastIndex = TargetArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				const_cast<TArray<T>*>(&TargetArray)->Swap(i, Index);
			}
		}
	}

	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr) return FString("Invalid");
		return enumPtr->GetNameByValue((int64)Value).ToString();
	}

	UFUNCTION(BlueprintCallable, Category = "Toolbox")
	static bool HasInvalidCharacters(FString str);

	// Get the player start with specific name in the scene
	static APlayerStart* GetPlayerStart(UWorld* World,  FName PlayerTag);

};
