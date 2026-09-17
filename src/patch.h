#pragma once

#include <SKSE/SKSE.h>
#include <xbyak/xbyak.h>

namespace Patches::EnableAchievementsWithMods
{
    namespace detail
    {
        struct Patch final : Xbyak::CodeGenerator
        {
            Patch()
            {
                xor_(rax, rax);
                ret();
            }
        };
    }

    inline void Install()
    {
        REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(13647, 441528) };

        // Zero-fill/NOP 5 bytes at target address using standard NOP byte 0x90
        target.write_fill(0x90, 0x05);

        detail::Patch p;
        target.write(std::span{ p.getCode<const std::byte*>(), p.getSize() });

        SKSE::log::info("Installed enable achievements with mods patch");
    }
}
