---------------------------------
--! @file xmake.lua
--! @author Argore
--! @brief Template C++ configuration
--! @version 0.1
--! @date 2025-04-04
--! 
--! @copyright Copyright (c) 2025 Argore
---------------------------------

set_languages("clatest", "cxxlatest")

add_rules "mode.debug"
add_rules "mode.release"

target "C++ Project"
    set_kind "binary" 
    
    set_basename "cpp_project_$(mode)_$(arch)"

    set_targetdir "bin"
    set_objectdir "build/obj"
    
    add_includedirs "src/"
    add_headerfiles{ "src/**.h", "src/**.hpp" }
    add_files{ "src/**.c", "src/**.cpp" }
target_end()
