using UnrealBuildTool;

public class GeneticAlgorithmModule : ModuleRules
{
	public GeneticAlgorithmModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string [] { "Core", "CoreUObject", "Engine"} );
		
		PublicIncludePaths.AddRange(new string[] {"GeneticAlgorithmModule/Public"});
		
		PrivateIncludePaths.AddRange(new string[] {"GeneticAlgorithmModule/Private"});
	}
}