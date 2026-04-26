// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdvancedUnrealCPP : ModuleRules
{
	public AdvancedUnrealCPP(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayTasks",
			"NavigationSystem",
			"Niagara",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AdvancedUnrealCPP",
			"AdvancedUnrealCPP/Variant_Platforming",
			"AdvancedUnrealCPP/Variant_Platforming/Animation",
			"AdvancedUnrealCPP/Variant_Combat",
			"AdvancedUnrealCPP/Variant_Combat/AI",
			"AdvancedUnrealCPP/Variant_Combat/Animation",
			"AdvancedUnrealCPP/Variant_Combat/Gameplay",
			"AdvancedUnrealCPP/Variant_Combat/Interfaces",
			"AdvancedUnrealCPP/Variant_Combat/UI",
			"AdvancedUnrealCPP/Variant_SideScrolling",
			"AdvancedUnrealCPP/Variant_SideScrolling/AI",
			"AdvancedUnrealCPP/Variant_SideScrolling/Gameplay",
			"AdvancedUnrealCPP/Variant_SideScrolling/Interfaces",
			"AdvancedUnrealCPP/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
