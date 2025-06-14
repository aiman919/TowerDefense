// Source/MyProject/MyProject.Build.cs
using UnrealBuildTool;
using System.IO;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",

            // Gameplay
            "AIModule",
            "NavigationSystem",
            "GameplayTasks",
            "EnhancedInput",
            "Niagara"
        });

        // Если когда-нибудь понадобится C++-UI:
        // PrivateDependencyModuleNames.AddRange(new[] { "UMG", "Slate", "SlateCore" });

        // include-пути с заголовками модуля
        PublicIncludePaths.AddRange(new[]
        {
            Path.Combine(ModuleDirectory, "Public")
        });
    }
}
