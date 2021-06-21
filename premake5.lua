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

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	group "Dependencies"
		include "NutLib/vendor/Glad"
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
		"%{prj.location}/%{IncludeDir.glm}"
	}

	links
	{
		"opengl32.lib",
		"Glad"
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
	
	filter "configurations:Release"
		optimize "on"
	


	
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
		"%{wks.location}/NutLib/%{IncludeDir.glm}"
	}
	
	links 
	{
		"NutLib"
	}
	
	filter "configurations:Debug"
		defines "NUT_DEBUG"
		symbols "on"
	
	filter "configurations:Release"
		optimize "on"
