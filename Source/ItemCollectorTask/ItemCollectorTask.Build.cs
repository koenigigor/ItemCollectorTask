// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ItemCollectorTask : ModuleRules
{
	public ItemCollectorTask(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "NetCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"ItemCollectorTask",
			"ItemCollectorTask/Misc"
		});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
