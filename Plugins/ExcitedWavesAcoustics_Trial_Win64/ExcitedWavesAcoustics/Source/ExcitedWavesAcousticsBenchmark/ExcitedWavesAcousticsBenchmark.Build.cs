using UnrealBuildTool;

public class ExcitedWavesAcousticsBenchmark : ModuleRules
{
    public ExcitedWavesAcousticsBenchmark(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "ExcitedWavesAcoustics"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );
    }
}
