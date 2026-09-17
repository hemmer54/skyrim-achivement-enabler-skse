add_rules("mode.release", "mode.debug")

set_languages("c++23")

add_requires("xbyak")

includes("lib/commonlibsse-ng")

target("EnableAchievementsWithMods")
    set_kind("shared")
    add_deps("commonlibsse-ng")
    add_packages("xbyak")

    add_rules("commonlibsse-ng.plugin", {
        name = "EnableAchievementsWithMods",
        author = "hemmer54",
        description = "Enables achievements when mods are active"
    })

    add_files("src/*.cpp")
    add_headerfiles("src/*.h")
