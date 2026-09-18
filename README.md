# Achievements Enabler

A native script-extender plugin that keeps Steam achievements enabled while mods are active.

## Supported games

- Skyrim SE/AE/VR through CommonLibSSE-NG and SKSE
- Fallout 4 through CommonLibF4 and F4SE

The Fallout 4 backend supports the known old-gen, Creators Club, and Next-Gen executable layouts. It scans the executable's `.text` section for a version-specific signature and refuses to patch unknown layouts instead of writing to an unsafe address.

## Build

Initialize all dependencies once:

```text
git submodule update --init --recursive
```

Build Skyrim:

```text
xmake f -p windows -a x64 --game=skyrim
xmake
```

Build Fallout 4:

```text
xmake f -p windows -a x64 --game=fallout4
xmake
```

The selected build produces a game-specific DLL:

- Skyrim: `EnableAchievementsWithModsSKSE.dll`
- Fallout 4: `EnableAchievementsWithModsF4SE.dll`

The resulting DLL is packaged by the selected CommonLib plugin rule for the appropriate extender directory.

## Runtime requirements

- Skyrim: SKSE and the matching Skyrim runtime/address library
- Fallout 4: F4SE and a supported Fallout 4 executable

An unsupported game executable is logged and left unmodified.
