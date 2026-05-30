// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StealthSurvival : ModuleRules
{
	public StealthSurvival(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"StealthSurvival",
			"StealthSurvival/Variant_Platforming",
			"StealthSurvival/Variant_Platforming/Animation",
			"StealthSurvival/Variant_Combat",
			"StealthSurvival/Variant_Combat/AI",
			"StealthSurvival/Variant_Combat/Animation",
			"StealthSurvival/Variant_Combat/Gameplay",
			"StealthSurvival/Variant_Combat/Interfaces",
			"StealthSurvival/Variant_Combat/UI",
			"StealthSurvival/Variant_SideScrolling",
			"StealthSurvival/Variant_SideScrolling/AI",
			"StealthSurvival/Variant_SideScrolling/Gameplay",
			"StealthSurvival/Variant_SideScrolling/Interfaces",
			"StealthSurvival/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
