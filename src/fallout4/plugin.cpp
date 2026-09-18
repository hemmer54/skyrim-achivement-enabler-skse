#include <F4SE/F4SE.h>

#include "patch.h"

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
    F4SE::Init(a_f4se);
    REX::INFO("Achievements Enabler loading for Fallout 4...");

    if (!Achievements::Fallout4::Install()) {
        REX::WARN("Could not find a supported Fallout 4 achievements guard; no patch was installed");
        return false;
    }

    REX::INFO("Fallout 4 achievements enabled while mods are active");
    return true;
}
