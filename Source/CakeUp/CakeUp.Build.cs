using UnrealBuildTool;

public class CakeUp : ModuleRules
{
    public CakeUp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "Slate",
            "SlateCore",
            "AnimGraphRuntime"   // ← ВАЖНО: добавили
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            // пока пусто
        });
    }
}
