project "LearnOpenGL"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("../bin/" .. outputdir)
	objdir ("../bin-int/" .. outputdir)

	pchheader "glpch.h"
	pchsource "src/glpch.cpp"

	files 
	{
		"src/**.cpp",
		"src/**.h",

		"Dependencies/stb_image/**.h",
		"Dependencies/stb_image/**.cpp",
		"Dependencies/glm/glm/**.hpp",
		"Dependencies/glm/glm/**.inl",
		"Dependencies/imgui/**.h",
		"Dependencies/imgui/**.cpp",
		"Dependencies/FreeType/include/**.h"
	}

	includedirs
	{
		"src",
		"Dependencies",
		"Dependencies/imgui",
		"Dependencies/stb_image",
		"Dependencies/GLFW/include",
		"Dependencies/GLEW/include",
		"Dependencies/glm",
		"Dependencies/FreeType/include",
		"Dependencies/assimp/include"
	}

	libdirs
	{
		"Dependencies/GLFW/lib/x64",
		"Dependencies/GLEW/lib/x64",
		"Dependencies/FreeType/lib/x64",
		"Dependencies/assimp/lib/x64"
	}

	links
	{
		"freetype.lib",
		"assimp-vc143-mt.lib",
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib"
	}

	-- needed because assimp dll needs to exist where the exe is
	postbuildcommands
    {
        '{COPY} "%{wks.location}/%{prj.name}/Dependencies/assimp/lib/x64/assimp-vc143-mt.dll" "%{cfg.targetdir}"'
    }

	defines
	{
		"GLEW_STATIC",
		"_CONSOLE"
	}

	filter 
	{
		"files:Dependencies/glm/**.cpp or files:Dependencies/stb_image/**.cpp or files:Dependencies/imgui/**.cpp"
	}
	flags "NoPCH"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"