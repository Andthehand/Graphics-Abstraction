 include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "TestBed"
	architecture "x86_64"
	startproject "VulkanTest"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	solution_items {
		".editorconfig"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "VulkanTest/vendor/GLFW"
group ""

include "VulkanTest"