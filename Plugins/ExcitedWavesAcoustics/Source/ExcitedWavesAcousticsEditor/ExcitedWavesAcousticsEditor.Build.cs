using UnrealBuildTool;

public class ExcitedWavesAcousticsEditor : ModuleRules
{
    public ExcitedWavesAcousticsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDefinitions.Add("EW_ACOUSTICS_TRIAL=1");

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add other public include paths required here ...
			}
        );

        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "ExcitedWavesAcoustics",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "EditorStyle",
                "PropertyEditor",
                "UnrealEd",
                "EditorSubsystem",
                "HTTP",
                "Json",
                "Projects",
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
        );
    }
}
