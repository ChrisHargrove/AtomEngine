#pragma once

#include <vector>

namespace Kernel {
    std::vector<float> Identity = {
        0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f
    };

    std::vector<float> Sharpen = {
        -1.0f,-1.0f,-1.0f,
        -1.0f, 9.0f,-1.0f,
        -1.0f,-1.0f,-1.0f
    };

    std::vector<float> Blur = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };

    std::vector<float> EdgeDetection = {
        1.0f, 1.0f, 1.0f,
        1.0f,-8.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    std::vector<float> SobelEdgeH = {
        -1.0f,-2.0f,-1.0f,
        0.0f,0.0f,0.0f,
        1.0f,2.0f,1.0f
    };

    std::vector<float> SobelEdgeV = {
        -1.0f,0.0f,1.0f,
        -2.0f,0.0f,2.0f,
        -1.0f,0.0f,1.0f
    };

    std::vector<float> Laplacian = {
        0.0f,-1.0f,0.0f,
        -1.0f, 4.0f, -1.0f,
        0.0f,-1.0f,0.0f
    };

    std::vector<float> LaplacianDiag = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,8.0f,-1.0f,
        -1.0f,-1.0f,-1.0f
    };
}
