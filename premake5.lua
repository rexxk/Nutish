-- premake5.lua
-- Main workspace

workspace "Nutish"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	startproject "Sandbox"

	IncludeDir = {}
	IncludeDir["Glad"] = "vendor/Glad/include"
	IncludeDir["glm"] = "vendor/glm"
	IncludeDir["spdlog"] = "vendor/spdlog/include"
	IncludeDir["imgui"] = "vendor/imgui"
	IncludeDir["stbimage"] = "vendor/stb-image"
	IncludeDir["assimp"] = "vendor/assimp/include"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	group "Dependencies"
		include "NutLib/vendor/Glad"
		include "NutLib/vendor/imgui"
	group ""


project "NutLib"
	location "NutLib"
	kind "StaticLib"
	language "C++"
	cppdialect "c++20"

	
	files
	{
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.location}/src",
		"%{prj.location}/%{IncludeDir.spdlog}",
		"%{prj.location}/%{IncludeDir.Glad}",
		"%{prj.location}/%{IncludeDir.glm}",
		"%{prj.location}/%{IncludeDir.imgui}",
		"%{prj.location}/%{IncludeDir.stbimage}",
		"%{prj.location}/%{IncludeDir.assimp}"
	}

	links
	{
		"opengl32.lib",
		"Glad",
		"ImGui"
	}

	pchheader "nutpch.h"
	pchsource "%{prj.location}/src/nutpch.cpp"

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "configurations:Debug"
		defines "NUT_DEBUG"
		symbols "on"

		links
		{
			"Nutlib/vendor/assimp/lib/assimp-vc142-mtd.lib"
		}
	
	filter "configurations:Release"
		optimize "on"

		links
		{
			"Nutlib/vendor/assimp/lib/assimp-vc142-mt.lib"
		}
	


	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++20"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	
	files 
	{
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp"
	}

	includedirs 
	{
		"%{wks.location}/NutLib/src",
		"%{wks.location}/NutLib/%{IncludeDir.spdlog}",
		"%{wks.location}/NutLib/%{IncludeDir.Glad}",
		"%{wks.location}/NutLib/%{IncludeDir.glm}",
		"%{wks.location}/NutLib/%{IncludeDir.imgui}"
	}
	
	links 
	{
		"NutLib"
	}
	
	filter "configurations:Debug"
		defines "NUT_DEBUG"
		symbols "on"

		postbuildcommands
		{
			'{COPY} "../Nutlib/vendor/assimp/bin/assimp-vc142-mtd.dll" "%{cfg.targetdir}"'
		}
	
	filter "configurations:Release"
		optimize "on"

		postbuildcommands
		{
			'{COPY} "../Nutlib/vendor/assimp/bin/assimp-vc142-mt.dll" "%{cfg.targetdir}"'
		}
