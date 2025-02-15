// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ReapTheUndead : ModuleRules
{
	public ReapTheUndead(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "UMG", "Slate", "SlateCore", "Json" });
	}
}
