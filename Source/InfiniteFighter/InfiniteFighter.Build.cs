// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InfiniteFighter : ModuleRules
{
	public InfiniteFighter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "InfiniteFighter" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", 
			"AnimGraphRuntime", "CommonInput", "UMG", "MotionWarping", "LevelSequence", "MovieScene", "NavigationSystem", "AIModule", "GameplayTasks", "GameplayTags" });
	}
}
