cmake_minimum_required(VERSION 3.0.0...3.24 FATAL_ERROR)
project(webgpu-backend-dawn VERSION 1.0.0)

message(STATUS "Using Dawn backend for WebGPU")

add_library(webgpu INTERFACE
        ../plugin/ui/WebGpuWindow.cpp
        ../plugin/ui/WebGpuWindow.h
		../plugin/ui/Utilities.h
		../plugin/ui/Scene.cpp
		../plugin/ui/Scene.h
		../plugin/ui/ResourceManager.h
		../plugin/geometries/PlaneGeometry.cpp
		../plugin/geometries/PlaneGeometry.h
        ../plugin/ui/cameraState.h
		../plugin/ui/cameraHelper.h
)

if (EMSCRIPTEN)

	target_include_directories(webgpu INTERFACE
		"include-emscripten"
	)

	# This is used to advertise the flavor of WebGPU that this zip provides
	target_compile_definitions(webgpu INTERFACE WEBGPU_BACKEND_EMSCRIPTEN)

	target_link_options(webgpu INTERFACE
		-sUSE_WEBGPU # Handle WebGPU symbols
	)

else (EMSCRIPTEN)

	include(cmake/FetchDawn.cmake)

	target_link_libraries(webgpu INTERFACE webgpu_dawn)
	target_include_directories(webgpu INTERFACE
		"include"
		"${CMAKE_BINARY_DIR}/_deps/dawn-src/include"
	)

	# This is used to advertise the flavor of WebGPU that this zip provides
	target_compile_definitions(webgpu INTERFACE WEBGPU_BACKEND_DAWN)

endif (EMSCRIPTEN)

# Dawn is built as a shared library (webgpu_dawn), so on Windows the DLL must sit
# next to each built binary. Covers the JUCE format targets (<Target>_Standalone, ...).
function(target_copy_webgpu_binaries Target)
	if (WIN32)
		foreach(Format Standalone VST3)
			if (TARGET ${Target}_${Format})
				add_custom_command(TARGET ${Target}_${Format} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
						$<TARGET_FILE:webgpu_dawn>
						$<TARGET_FILE_DIR:${Target}_${Format}>
					COMMENT "Copying webgpu_dawn.dll next to ${Target}_${Format}"
				)
			endif()
		endforeach()
	endif()
endfunction()
