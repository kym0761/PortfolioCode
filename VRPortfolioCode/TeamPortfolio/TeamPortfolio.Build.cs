// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TeamPortfolio : ModuleRules
{
	public TeamPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
                                                            "AIModule", "GamePlayTasks", "UMG", "NavigationSystem", "Niagara" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
