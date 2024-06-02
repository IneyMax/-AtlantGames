// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AtlantGames : ModuleRules
{
	public AtlantGames(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"NetCore",
			"Networking",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
