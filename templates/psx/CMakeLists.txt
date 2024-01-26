cmake_minimum_required(VERSION 3.21)

project(
	game
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "game testing"
	HOMEPAGE_URL ""
)

file(GLOB _sources src/*.c)
psn00bsdk_add_executable(game GPREL ${_sources})
#psn00bsdk_add_cd_image(game_iso game iso.xml DEPENDS game)

include (texture_list.txt)

install(FILES ${PROJECT_BINARY_DIR}/game.exe TYPE BIN)
