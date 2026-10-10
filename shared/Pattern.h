/*
    Plugin-SDK (Grand Theft Auto) SHARED header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "DynAddress.h"
#include "Base.h"
#include "../injector/assembly.hpp"
#include "../injector/injector.hpp"
#include "../hooking/Hooking.Patterns.h"
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

namespace plugin {
    class pattern {
    private:
        static inline std::unique_ptr<std::unordered_map<std::string_view, uintptr_t>> patternMap;
        static inline std::unique_ptr<std::unordered_map<std::string_view, uintptr_t>> modulePatternMap;

        // `bytes` may hold one signature per game build, separated by '|':
        // "CE bytes | 1.0.8.0 bytes". The running build picks its alternative
        // by position; a pattern with fewer alternatives than that position
        // uses its last one, because a plain pattern is one that matches both.
        // An alternative may end in "@N" to shift its result (see below).
        //
        // Choosing by build rather than by "whichever matches" matters: a CE
        // signature can match once in 1.0.8.0 at a look-alike site (a setter
        // with the same body for a different flag), and that binds the wrong
        // address with nothing to show for it.
        template<typename Scan>
        static inline uintptr_t FindAlternative(std::string_view bytes, Scan scan) {
            size_t wanted = BuildAlternative();
            std::string_view alt;
            for (size_t index = 0; !bytes.empty(); ++index) {
                size_t bar = bytes.find('|');
                alt = bytes.substr(0, bar);
                bytes = bar == std::string_view::npos ? std::string_view{} : bytes.substr(bar + 1);
                if (index == wanted)
                    break;
            }

            // "bytes @N" moves the result N bytes on from the match, for a
            // site whose own bytes are not unique on that build (a tiny
            // function shared by several pools) but whose surroundings are.
            int32_t shift = 0;
            size_t at = alt.find('@');
            if (at != std::string_view::npos) {
                shift = static_cast<int32_t>(std::strtol(std::string(alt.substr(at + 1)).c_str(), nullptr, 0));
                alt = alt.substr(0, at);
            }

            size_t first = alt.find_first_not_of(' ');
            size_t last = alt.find_last_not_of(' ');
            if (first == std::string_view::npos)
                return 0x0;
            alt = alt.substr(first, last - first + 1);

            auto p = scan(alt);
            return p.empty() ? 0x0 : (uintptr_t)p.get_first(0) + shift;
        }

    public:
        // Position of this executable's alternative in a '|' pattern: 1 for
        // GTA IV 1.0.x, 0 for the Complete Edition and anything unrecognised.
        // Read from the executable's own version resource, so no file access.
        static inline size_t BuildAlternative() {
#ifdef GTAIV
            static const size_t index = [] {
                HMODULE game = GetModuleHandleW(nullptr);
                HRSRC info = FindResourceW(game, MAKEINTRESOURCEW(1), MAKEINTRESOURCEW(16));
                HGLOBAL loaded = info ? LoadResource(game, info) : nullptr;
                auto data = loaded ? static_cast<const uint8_t*>(LockResource(loaded)) : nullptr;
                DWORD size = info ? SizeofResource(game, info) : 0;
                for (DWORD offset = 0; data && offset + 16 <= size; offset += 4) {
                    DWORD signature, versionMS;
                    memcpy(&signature, data + offset, 4);
                    if (signature != 0xFEEF04BD)
                        continue;
                    memcpy(&versionMS, data + offset + 8, 4);
                    return (HIWORD(versionMS) == 1 && LOWORD(versionMS) == 0) ? size_t(1) : size_t(0);
                }
                return size_t(0);
            }();
            return index;
#else
            return 0;
#endif
        }

        static inline uintptr_t Get(std::string_view const& bytes, int32_t offset = 0) {
            if (!patternMap) {
                patternMap = std::make_unique<std::unordered_map<std::string_view, uintptr_t>>();
                patternMap->reserve(1024);
            }
            uintptr_t a = 0x0;
            auto it = patternMap->find(bytes);
            if (it != patternMap->end()) {
                a = it->second;
                goto return_addr;
            }
            a = FindAlternative(bytes, [](std::string_view alt) { return hook::pattern(alt); });
            patternMap->emplace(bytes, a);
        return_addr:
            // A miss must stay zero. Rebasing it first yields
            // STARTING_ADDRESS - base, which is non-zero whenever the game is
            // relocated, and Read() then dereferences address `offset`.
            if (!a)
                return 0x0;
            a -= GetBaseAddress();
            a += STARTING_ADDRESS;
            return a ? a + offset : 0x0;
        }

        static inline uintptr_t GetExternal(void* module, std::string_view const& bytes, int32_t offset = 0) {
            if (!modulePatternMap) {
                modulePatternMap = std::make_unique<std::unordered_map<std::string_view, uintptr_t>>();
                modulePatternMap->reserve(1024);
            }
            uintptr_t a = 0x0;
            auto it = modulePatternMap->find(bytes);
            if (it != modulePatternMap->end()) {
                a = it->second;
                goto return_addr;
            }
            a = FindAlternative(bytes, [module](std::string_view alt) { return hook::module_pattern(module, alt); });
            modulePatternMap->emplace(bytes, a);
return_addr:
            if (!a)
                return 0x0;
            a -= GetBaseAddress();
            a += STARTING_ADDRESS;
            return a ? a + offset : 0x0;
        }

        template<typename T = void*>
        static inline auto Read(std::string_view const& bytes, int32_t offset = 0) {
            uintptr_t const& a = Get(bytes, offset);

#if (defined (_M_IX86) || defined (_X86_))
            return a ? injector::ReadMemory<T>(GetGlobalAddress(a), true) : 0x0;
#else
            return a ? (T)injector::ReadRelativeOffset(GetGlobalAddress(a), 4, true).as_int() : 0x0;
#endif
        }

        template<typename T = void*>
        static inline auto ReadExternal(void* module, std::string_view const& bytes, int32_t offset = 0) {
            uintptr_t const& a = GetExternal(module, bytes, offset);

#if (defined (_M_IX86) || defined (_X86_))
            return a ? injector::ReadMemory<T>(GetGlobalAddress(a), true) : 0x0;
#else
            return a ? (T)injector::ReadRelativeOffset(GetGlobalAddress(a), 4, true).as_int() : 0x0;
#endif
        }
    };
}

#define gpattern(bytes) plugin::pattern::Get(bytes, 0)
#define gpatternt(t, bytes, offset) plugin::pattern::Read<t*>(bytes, offset)