// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NBC : ModuleRules
{
	public NBC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG"});
	}
}
