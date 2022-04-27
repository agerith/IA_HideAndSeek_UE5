// Fill out your copyright notice in the Description page of Project Settings.

#include "Toolbox.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SplineComponent.h"
#include "Logging/LogVerbosity.h"

#include "Internationalization/Regex.h"


//Incarna Log
DEFINE_LOG_CATEGORY(LogIncarnaEdit);
DEFINE_LOG_CATEGORY(LogIncarnaDev);
DEFINE_LOG_CATEGORY(LogIncarnaShip);


#pragma region PrintLog
void UToolbox::PrintLog(FString InString /*= "Hello"*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	if (PrintToScreen && GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(true), InString);
	}

	if (PrintToLog)
	{
		if (GetBuildConfiguration() == "SHIPPING")
		{
			switch (VerbosityLevel)
			{
			case EFatal:
				UE_LOG(LogIncarnaShip, Fatal, TEXT("%s"), *InString);
				break;
			case EError:
				UE_LOG(LogIncarnaShip, Error, TEXT("%s"), *InString);
				break;
			case EWarning:
				UE_LOG(LogIncarnaShip, Warning, TEXT("%s"), *InString);
				break;
			case EDisplay:
				UE_LOG(LogIncarnaShip, Display, TEXT("%s"), *InString);
				break;
			case ELog:
				UE_LOG(LogIncarnaShip, Log, TEXT("%s"), *InString);
				break;
			case EVerbose:
				UE_LOG(LogIncarnaShip, Verbose, TEXT("%s"), *InString);
				break;
			case EVeryVerbose:
				UE_LOG(LogIncarnaShip, VeryVerbose, TEXT("%s"), *InString);
				break;
			}
		}
		else if (GetBuildConfiguration() == "DEVELOPMENT")
		{
			switch (VerbosityLevel)
			{
			case EFatal:
				UE_LOG(LogIncarnaDev, Fatal, TEXT("%s"), *InString);
				break;
			case EError:
				UE_LOG(LogIncarnaDev, Error, TEXT("%s"), *InString);
				break;
			case EWarning:
				UE_LOG(LogIncarnaDev, Warning, TEXT("%s"), *InString);
				break;
			case EDisplay:
				UE_LOG(LogIncarnaDev, Display, TEXT("%s"), *InString);
				break;
			case ELog:
				UE_LOG(LogIncarnaDev, Log, TEXT("%s"), *InString);
				break;
			case EVerbose:
				UE_LOG(LogIncarnaDev, Verbose, TEXT("%s"), *InString);
				break;
			case EVeryVerbose:
				UE_LOG(LogIncarnaDev, VeryVerbose, TEXT("%s"), *InString);
				break;
			}
		}
		else
		{
			switch (VerbosityLevel)
			{
			case EFatal:
				UE_LOG(LogIncarnaEdit, Fatal, TEXT("%s"), *InString);
				break;
			case EError:
				UE_LOG(LogIncarnaEdit, Error, TEXT("%s"), *InString);
				break;
			case EWarning:
				UE_LOG(LogIncarnaEdit, Warning, TEXT("%s"), *InString);
				break;
			case EDisplay:
				UE_LOG(LogIncarnaEdit, Display, TEXT("%s"), *InString);
				break;
			case ELog:
				UE_LOG(LogIncarnaEdit, Log, TEXT("%s"), *InString);
				break;
			case EVerbose:
				UE_LOG(LogIncarnaEdit, Verbose, TEXT("%s"), *InString);
				break;
			case EVeryVerbose:
				UE_LOG(LogIncarnaEdit, VeryVerbose, TEXT("%s"), *InString);
				break;
			}
		}
	}
}

void UToolbox::PrintLog(const char* InString /*= "Hello"*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = FString(InString);
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

void UToolbox::PrintLog(float InString /*= 0.0f*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = FString::SanitizeFloat(InString);
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

void UToolbox::PrintLog(int InString /*= 0*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = FString::FromInt(InString);
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

void UToolbox::PrintLog(bool InString /*= true*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = InString ? "true" : "false";
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

void UToolbox::PrintLog(FVector InString /*= FVector::ZeroVector*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = InString.ToString();
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

void UToolbox::PrintLog(FRotator InString /*= FRotator::ZeroRotator*/, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = InString.ToString();
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

void UToolbox::PrintLog(UObject* InString, bool PrintToScreen /*= true*/, bool PrintToLog /*= true*/, ELogVerbosityType VerbosityLevel /*= EDisplay*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.0f*/)
{
	FString inString = (InString != nullptr) ? InString->GetName() : "None";
	PrintLog(inString, PrintToScreen, PrintToLog, VerbosityLevel, TextColor, Duration);
}

#pragma endregion

FString UToolbox::GetBuildConfiguration()
{
	#if WITH_EDITOR
		if (!(GEditor->bIsSimulatingInEditor || (GEditor->PlayWorld != NULL)))
		{
			return "EDITOR";
		} 
		else
		{
			return "EDITOR_PLAY";
		}
	#else
		#if !UE_BUILD_SHIPPING
			return "DEVELOPMENT";
		#else
			return "SHIPPING";
		#endif
	#endif
}

FSplinePoint UToolbox::CopySplinePoint(USplineComponent* SplineComponent, int IndexPoint, bool IsLocal /*= true*/)
{
	FSplinePoint NewPoint;
	ESplineCoordinateSpace::Type CoordinateSpace = (IsLocal ? ESplineCoordinateSpace::Local : ESplineCoordinateSpace::World);
	NewPoint.InputKey = IndexPoint;
	NewPoint.Position = SplineComponent->GetTransformAtSplinePoint(IndexPoint, CoordinateSpace).GetLocation();
	NewPoint.Rotation = SplineComponent->GetTransformAtSplinePoint(IndexPoint, CoordinateSpace).GetRotation().Rotator();
	NewPoint.Scale = SplineComponent->GetTransformAtSplinePoint(IndexPoint, CoordinateSpace).GetScale3D();
	NewPoint.ArriveTangent = SplineComponent->GetArriveTangentAtSplinePoint(IndexPoint, CoordinateSpace);
	NewPoint.LeaveTangent = SplineComponent->GetLeaveTangentAtSplinePoint(IndexPoint, CoordinateSpace);
	NewPoint.Type = SplineComponent->GetSplinePointType(IndexPoint);
	return NewPoint;
}

FColor UToolbox::MakeSimpleColor(FLinearColor SimpleColor /*= FColor::White*/)
{
	return SimpleColor.ToFColor(true);
}

bool UToolbox::HasInvalidCharacters(FString str)
{
	

	const FRegexPattern myPattern(TEXT("[a-zA-Z0-9-_]"));

	FRegexMatcher myMatcher(myPattern, str);

	int32 beginIndex = -1;
	int32 endingIndex = -1;
	int32 lastEnding = 0;

	while (myMatcher.FindNext())
	{	
		if (beginIndex == -1) {
			beginIndex = myMatcher.GetMatchBeginning();
		}

		endingIndex = myMatcher.GetMatchEnding();


		if (endingIndex > 0) {
			if (endingIndex != lastEnding + 1)
			{
				return true;
			}
		}

		lastEnding = endingIndex;
	}

	if (beginIndex != 0 || endingIndex != str.Len()) {
		return true;
	}


	return false;

	/*
	const FRegexPattern myPattern(TEXT("[$&+,:;=?@#| '<>.^*()%!]"));
	//const FRegexPattern myPattern(TEXT("[$&+,:;=?@#|'<>.^*()%! àèìòùÀÈÌÒÙáéíóúýÁÉÍÓÚÝ âêîôûÂÊÎÔÛãñõÃÑÕäëïöüÿÄËÏÖÜŸçÇßØøÅåÆæœµ£¤°`\\\"§²]"));

	FRegexMatcher myMatcher(myPattern, str);


	if (myMatcher.FindNext())
	{
		return true;
	}
	else {

		const FRegexPattern myPattern2(TEXT("[àèé]"));
		FRegexMatcher myMatcher2(myPattern2, str);
		if (myMatcher2.FindNext())
		{
			return true;
		}
		else {
			return false;
		}
	}*/
}

APlayerStart* UToolbox::GetPlayerStart(UWorld* World, FName PlayerTag)
{
	if (!World) {
		PrintLog("[Tool]  => THE WORLD IS NOT VALID (ToolBox::GetPlayerStart)", true, true, EError);
		return NULL;
	}
	else {
		TArray<AActor*> playerStarts;
		UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), playerStarts);
		if (playerStarts.Num() == 0) {
			PrintLog("[Tool]  => No player start find (ToolBox::GetPlayerStart)", true, true, EError);
			return NULL;
		}
		else {
			for (AActor* pStart : playerStarts)
			{
				APlayerStart* playerStart = (APlayerStart*)pStart;
				if (playerStart->PlayerStartTag == PlayerTag) {
					return playerStart;
				}
			}
			PrintLog("[Tool]  => No player start has the name search (ToolBox::GetPlayerStart)", true, true, EError);
			return NULL;
		}
	}
}