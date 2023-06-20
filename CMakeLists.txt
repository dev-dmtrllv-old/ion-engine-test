cmake_minimum_required (VERSION 3.8)

project(ion)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

# Ion static lib
file(GLOB_RECURSE ion_sources "src/*.cpp" "include/*.hpp" "src/js/*.cpp" "include/js/*.hpp")
list(REMOVE_ITEM ${ion_sources} "include/pch.hpp")
add_executable(ion WIN32 ${ion_sources})
target_include_directories(ion PRIVATE "include" ${OPENGL_INCLUDE_DIRS})
target_precompile_headers(ion PRIVATE "include/pch.hpp")
target_link_libraries(ion ${OPENGL_LIBRARIES})
# End ion static lib



# # Test game config
# file(GLOB_RECURSE test_game_sources "test-game/src/*.cpp" "test-game/include/*.hpp")
# list(REMOVE_ITEM ${test_game_sources} "test-game/include/pch.hpp")
# add_executable(test_game WIN32 ${test_game_sources})
# target_include_directories(test_game PRIVATE "test-game/include" "include")
# target_precompile_headers(test_game PRIVATE "include/pch.hpp")
# target_link_libraries(test_game PRIVATE ion)
# # End test game config



# Dependencies config
# set the v8 include dir
target_include_directories(ion SYSTEM PRIVATE ./dependencies/includes/v8)

# set the v8 defines
add_definitions(-DV8_COMPRESS_POINTERS -DV8_31BIT_SMIS_ON_64BIT_ARCH -D_ITERATOR_DEBUG_LEVEL=0)

if(UNIX)
	add_definitions(-DV8_ENABLE_SANDBOX)
endif()

# set the v8 library target directory
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(V8_TARGET "x64.debug")
else()
	set(V8_TARGET "x64.release")
endif()

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	target_link_libraries(ion ${PROJECT_SOURCE_DIR}/dependencies/win/v8/${V8_TARGET}/obj/v8_monolith.lib)
	target_link_libraries(ion ${PROJECT_SOURCE_DIR}/dependencies/win/v8/${V8_TARGET}/obj/v8_libbase.lib)
	target_link_libraries(ion ${PROJECT_SOURCE_DIR}/dependencies/win/v8/${V8_TARGET}/obj/v8_libplatform.lib)
elseif(UNIX)
	target_link_libraries(ion ${PROJECT_SOURCE_DIR}/dependencies/linux/v8/${V8_TARGET}/libv8_libbase.a)
	target_link_libraries(ion ${PROJECT_SOURCE_DIR}/dependencies/linux/v8/${V8_TARGET}/libv8_libplatform.a)
	target_link_libraries(ion ${PROJECT_SOURCE_DIR}/dependencies/linux/v8/${V8_TARGET}/libv8_monolith.a)
endif()
# End dependencies config



# GLOBAL CONFIGS
if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE -D_CRT_SECURE_NO_WARNINGS)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
	target_link_libraries(ion X11)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET ion PROPERTY CXX_STANDARD 23)
	# set_property(TARGET test_game PROPERTY CXX_STANDARD 23)
endif()
# END GLOBAL CONFIGS