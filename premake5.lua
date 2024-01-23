workspace "templater"
	configurations { "Debug", "Release" }
	platforms { "x64" }

project "templater"
	kind "ConsoleApp"
	language "C"
	
	links { "Shlwapi.lib" }

	targetdir "bin"
	objdir "bin/obj/"

	files { 
		"src/**.h", 
		"src/**.c" 
	}
