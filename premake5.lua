project "Arrowhead"
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

    removefiles
    {
        "src/Platform/**.h",
		"src/Platform/**.cpp"
    }

    includedirs
    {
        "src",
        "include"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "src/Platform/Windows/**.h",
            "src/Platform/Windows/**.cpp",
        }

        defines
		{
			"ARWH_WINDOWS",
			"ARWH_MSVC"
		}

    filter "system:linux"
        files
        {
			"%{prj.name}/src/Arrow/Platform/Linux/**.h",
			"%{prj.name}/src/Arrow/Platform/Linux/**.cpp"
        }

        links
        {
            "xcb"
        }

        defines
        {
            "ARWH_LINUX",
			"ARWH_GCC"
        }

    filter "system:macosx"
		systemversion "12.7:latest"

		files
		{
			"%{prj.name}/src/Arrow/Platform/MacOS/**.h",
			"%{prj.name}/src/Arrow/Platform/MacOS/**.cpp",
			"%{prj.name}/src/Arrow/Platform/MacOS/**.mm"
		}

        links
		{
			"Cocoa.framework"
		}

		defines
		{
			"ARWH_MACOS",
			"ARWH_CLANG"
		}

    filter "configurations:Debug"
        defines "ARWH_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ARWH_RELEASE"
        runtime "Release"
        optimize "Speed"

    filter "configurations:Dist"
        defines "ARWH_DIST"
        runtime "Release"
        optimize "Speed"
        symbols "off"

PACKAGE_DIRS["arrowhead"] = path.getabsolute(".")