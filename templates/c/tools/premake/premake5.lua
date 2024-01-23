local path = require "./path"
PROJECT_NAME = path.getProjectName( 2 )

workspace (PROJECT_NAME)
	configurations { "Debug", "Release" }
	platforms { "x64" }
	location "../../"

project (PROJECT_NAME)
	kind "ConsoleApp"
	language "C"
	cdialect "Default"
	
	targetdir "../../bin"
	objdir "../../bin/obj/"

	location "../../build"

	files { 
		"../../src/**.h", 
		"../../src/**.c" 
	}
