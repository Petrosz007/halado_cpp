# Haladó C++ [![codecov](https://codecov.io/gh/Petrosz007/halado_cpp/branch/main/graph/badge.svg?token=5WL3WOWD5D)](https://codecov.io/gh/Petrosz007/halado_cpp) [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=Petrosz007_halado_cpp&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=Petrosz007_halado_cpp)

## Development
The project uses CMAKE, you can build it with

    mkdir -p build
    cd build
    cmake --build . --target test_main --

The project has every id_bimap version inside it. To switch between the tested versions, change the `BIMAP_VERSION` in the `test/sut.hpp`. You can also toggle `NO_FAIL` there.

The project has a github repository with a fully working CI, SonarCloud and Code Coverage reporting: https://github.com/Petrosz007/halado_cpp

## Dev Container
There is a VS Code devcontainer preconfigured for the project. To use it, open it in VS Code and with the `Remote - Containers` extension use `> Remote Containers: Open folder in container`

### Building the CI/CD container
Github actions use the same container as the dev environment. It needs to be built and pushed to Docker Hub whenever it is updated

    docker build -t andipeter/halado-cpp:latest .\.devcontainer\
    docker push andipeter/halado-cpp:latest

## Code formatting
The project uses `clang-format` to format the code automatically. This config can be changed using this awesome website: https://zed0.co.uk/clang-format-configurator/
