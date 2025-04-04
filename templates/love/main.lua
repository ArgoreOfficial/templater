---------------------------------
--! @file main.lua
--! @author Argore
--! @brief Template LÖVE2D Project
--! @version 0.1
--! @date 2025-04-04
--! 
--! @copyright Copyright (c) 2025 Argore
---------------------------------

local color = {0,0,0,1}
local clock = 0.0

function love.load()	
	love.window.setTitle( "LÖVE2D Project" )
end

function love.update(_dt)
	clock = clock + _dt

	color[ 1 ] = math.sin( clock ) * 0.5 + 0.5
	color[ 3 ] = math.cos( clock ) * 0.5 + 0.5
end

function love.draw()
	love.graphics.clear(unpack(color))
end