---------------------------------
--! @file main.lua
--! @author Argore
--! @brief Template LÖVE2D Project
--! @version 0.2
--! @date 2025-09-05
--! 
--! @copyright Copyright (c) 2025 Argore
---------------------------------

local r,g,b = 0,0,0
local clock = 0.0

table.unpack = unpack 

function love.load()	
	
end

function love.update(_dt)
	clock = clock + _dt

	r = math.sin( clock ) * 0.5 + 0.5
	g = math.cos( clock ) * 0.5 + 0.5
end

function love.keypressed(key, scancode, isrepeat)
	
end

function love.keyreleased(key, scancode, isrepeat)
	
end

function love.draw()
	love.graphics.clear(r,g,b,1)
end