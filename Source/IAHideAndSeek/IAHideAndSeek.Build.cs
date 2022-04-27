// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IAHideAndSeek : ModuleRules
{
	public IAHideAndSeek(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.bBuildEditor)
        {
            MinFilesUsingPrecompiledHeaderOverride = 1;
            bUseUnity = false;
            PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "EditorScriptingUtilities" });
        }

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "Niagara", "UMG", "Slate" });


        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
