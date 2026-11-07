//
// Created by Erik Jourgensen on 10/28/26.
//

#pragma once
#include <string>
#include <filesystem>
#include <iostream>

inline std::vector<std::filesystem::path> getShaderPaths() {
    std::cout << " Calling shader paths! " << std::endl;

    const std::string dir = DEBUG_SHADER_DIR;
    std::vector<std::filesystem::path> shaderPaths = {
        dir + "/vertex_main.wgsl",
        dir + "/fragment_main.wgsl",
        dir + "/common.wgsl",
        dir + "/mat_onOffButton.wgsl",
        dir + "/topLeftCube.wgsl",
        dir + "/topCenterCube.wgsl",
        dir + "/topRightCube.wgsl",
        dir + "/bottomLeftCube.wgsl",
        dir + "/bottomCenterCube.wgsl",
        dir + "/bottomRightCube.wgsl"


    };
    return shaderPaths;
}
