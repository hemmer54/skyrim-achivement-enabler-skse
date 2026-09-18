#pragma once

#include <windows.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

namespace Achievements::Fallout4
{
    namespace detail
    {
        struct Signature final
        {
            std::span<const std::byte> bytes;
            std::ptrdiff_t targetOffset;
        };

        inline std::byte* FindSignature(const std::byte* begin, const std::byte* end, const Signature& signature)
        {
            if (signature.bytes.empty() || begin >= end ||
                static_cast<std::size_t>(end - begin) < signature.bytes.size()) {
                return nullptr;
            }

            for (auto* current = begin; current <= end - signature.bytes.size(); ++current) {
                if (std::equal(signature.bytes.begin(), signature.bytes.end(), current)) {
                    return const_cast<std::byte*>(current + signature.targetOffset);
                }
            }
            return nullptr;
        }

        inline std::span<const std::byte> TextSection()
        {
            const auto module = reinterpret_cast<std::uintptr_t>(::GetModuleHandleW(nullptr));
            if (!module) {
                return {};
            }

            const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(module);
            if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
                return {};
            }

            const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(module + dos->e_lfanew);
            if (nt->Signature != IMAGE_NT_SIGNATURE) {
                return {};
            }

            const auto* section = IMAGE_FIRST_SECTION(nt);
            for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
                if (std::memcmp(section->Name, ".text", 5) == 0) {
                    return { reinterpret_cast<const std::byte*>(module + section->VirtualAddress),
                        section->Misc.VirtualSize };
                }
            }
            return {};
        }

        inline bool WriteReturnFalse(std::byte* target)
        {
            if (!target) {
                return false;
            }

            DWORD oldProtection{};
            if (!::VirtualProtect(target, 8, PAGE_EXECUTE_READWRITE, &oldProtection)) {
                return false;
            }

            constexpr std::array<std::byte, 8> patch{
                std::byte{ 0x48 }, std::byte{ 0x31 }, std::byte{ 0xC0 }, std::byte{ 0xC3 },
                std::byte{ 0x90 }, std::byte{ 0x90 }, std::byte{ 0x90 }, std::byte{ 0x90 }
            };
            std::copy(patch.begin(), patch.end(), target);
            ::FlushInstructionCache(::GetCurrentProcess(), target, patch.size());

            DWORD ignored{};
            ::VirtualProtect(target, 8, oldProtection, &ignored);
            return true;
        }
    }

    inline bool Install()
    {
        // The first two signatures cover pre-1.10 and 1.10-era Fallout 4.
        // The third is the function prologue used by the 1.10.980+ executable.
        static constexpr std::array<std::byte, 14> oldGen{
            std::byte{ 0xC3 }, std::byte{ 0x40 }, std::byte{ 0x32 }, std::byte{ 0xFF },
            std::byte{ 0x48 }, std::byte{ 0x89 }, std::byte{ 0x5C }, std::byte{ 0x24 },
            std::byte{ 0x40 }, std::byte{ 0x48 }, std::byte{ 0x89 }, std::byte{ 0x6C },
            std::byte{ 0x24 }, std::byte{ 0x48 }
        };
        static constexpr std::array<std::byte, 16> creatorsClub{
            std::byte{ 0xC3 }, std::byte{ 0xC6 }, std::byte{ 0x44 }, std::byte{ 0x24 },
            std::byte{ 0x38 }, std::byte{ 0x00 }, std::byte{ 0x48 }, std::byte{ 0x8D },
            std::byte{ 0x44 }, std::byte{ 0x24 }, std::byte{ 0x38 }, std::byte{ 0x48 },
            std::byte{ 0x89 }, std::byte{ 0x5C }, std::byte{ 0x24 }, std::byte{ 0x20 }
        };
        static constexpr std::array<std::byte, 14> nextGen{
            std::byte{ 0x48 }, std::byte{ 0x83 }, std::byte{ 0xEC }, std::byte{ 0x28 },
            std::byte{ 0xC6 }, std::byte{ 0x44 }, std::byte{ 0x24 }, std::byte{ 0x38 },
            std::byte{ 0x00 }, std::byte{ 0x84 }, std::byte{ 0xD2 }, std::byte{ 0x74 },
            std::byte{ 0x1C }, std::byte{ 0x48 }
        };

        const auto text = detail::TextSection();
        if (text.empty()) {
            return false;
        }

        const std::array<detail::Signature, 3> signatures{
            detail::Signature{ oldGen, -0x29 },
            detail::Signature{ creatorsClub, -0x28 },
            detail::Signature{ nextGen, 0 }
        };

        for (const auto& signature : signatures) {
            if (auto* target = detail::FindSignature(text.data(), text.data() + text.size(), signature);
                detail::WriteReturnFalse(target)) {
                return true;
            }
        }
        return false;
    }
}
