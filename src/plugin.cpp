#include <SKSE/SKSE.h>
#include "Patch.h"

namespace
{
    void Initialize()
    {
        Patches::EnableAchievementsWithMods::Install();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);

    SKSE::log::info("EnableAchievementsWithMods plugin loading...");

    Initialize();

    SKSE::log::info("EnableAchievementsWithMods plugin loaded successfully.");

    return true;
}
