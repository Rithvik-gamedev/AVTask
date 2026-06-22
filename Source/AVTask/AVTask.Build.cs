// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AVTask : ModuleRules
{
	public AVTask(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"NavigationSystem",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AVTask",
			"AVTask/Variant_Platforming",
			"AVTask/Variant_Platforming/Animation",
			"AVTask/Variant_Combat",
			"AVTask/Variant_Combat/AI",
			"AVTask/Variant_Combat/Animation",
			"AVTask/Variant_Combat/Gameplay",
			"AVTask/Variant_Combat/Interfaces",
			"AVTask/Variant_Combat/UI",
			"AVTask/Variant_SideScrolling",
			"AVTask/Variant_SideScrolling/AI",
			"AVTask/Variant_SideScrolling/Gameplay",
			"AVTask/Variant_SideScrolling/Interfaces",
			"AVTask/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
