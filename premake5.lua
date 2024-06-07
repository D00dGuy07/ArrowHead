include "vendor/glad"

project "oglfv2"
    language "C++"
    cppdialect "C++17"
    kind "StaticLib"
    
    staticruntime "on"
    systemversion "latest"

    targetdir (TARGET_DIR)
	objdir (OBJ_DIR)

    files
    {
        "include/**.h",
        "src/**.h",
		"src/**.cpp"
    }

    includedirs
    {
        "src",
        "include",
        path.join(PACKAGE_DIRS["glad"], "include"),
        path.join(PACKAGE_DIRS["glfw"], "include"),
        PACKAGE_DIRS["glm"],
        PACKAGE_DIRS["stb"]
    }

    links
    {
        "glad"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "GLDEBUG_OMIT_SEVERITY_NOTIFICATION"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "Speed"

    filter "configurations:Dist"
        runtime "Release"
        optimize "Speed"
        symbols "off"

PACKAGE_DIRS["oglfv2"] = path.getabsolute(".")