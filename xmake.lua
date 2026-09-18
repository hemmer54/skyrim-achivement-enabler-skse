set_project("AchievementsEnabler")
set_xmakever("2.8.0")
set_languages("c++23")

add_rules("mode.release", "mode.debug")
add_requires("xbyak")

option("game")
    set_default("skyrim")
    set_values("skyrim", "fallout4")
    set_showmenu(true)
option_end()

if is_config("game", "fallout4") then
    includes("lib/commonlibf4")

    target("EnableAchievementsWithModsF4SE")
        set_kind("shared")
        add_packages("xbyak")
        add_rules("commonlibf4.plugin", {
            name = "EnableAchievementsWithModsF4SE",
            author = "hemmer54",
            description = "Enables achievements when mods are active in Fallout 4"
        })
        add_files("src/fallout4/*.cpp")
        add_headerfiles("src/fallout4/*.h")
else
    includes("lib/commonlibsse-ng")

    target("EnableAchievementsWithModsSKSE")
        set_kind("shared")
        add_deps("commonlibsse-ng")
        add_packages("xbyak")
        add_rules("commonlibsse-ng.plugin", {
            name = "EnableAchievementsWithModsSKSE",
            author = "hemmer54",
            description = "Enables achievements when mods are active in Skyrim"
        })
        add_files("src/skyrim/*.cpp")
        add_headerfiles("src/skyrim/*.h")
end
