# antara-gaming-sdk
Antara Gaming Software Development Kit

## Prerequisites

Below is the list of prerequisites to use the `antara-gaming-sdk` on your machine:

-   [CMake](https://cmake.org/download/) 3.14 minimum
-   **clang-8** minimum (Windows/Linux/Osx) 
-   **Optional** emscripten latest (Web)
-   **Optional** Visual Studio 2019
-   **Optional** Clang VS Toolset (installable through visual studio installer)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

See deployment for notes on how to deploy the project on a live system.

### Build

To build the project please follow the instructions below:

```bash
mkdir build ## bash or powershell
cd build ## bash or powershell

## Release or Debug are available
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=your_path_to_your_clang++ ../ #Linux / Osx 
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -T "ClangCl" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" ../ #Windows

## Build (Debug / Release available)
cmake --build . --config Debug
```

### Installing

You do not need to install the gaming sdk, just use the CMake fetch_content command to use the project

## Running the tests

Once you have compiled the sdk gaming with the option to enable unit tests.

They are located in the `bin/unit_tests` (Linux/Osx) or `bin/unit_tests/%CMAKE_BUILD_TYPE%` (Windows) folder

## Deployment

:construction:

## Built With

-   [doctest](https://github.com/onqtam/doctest) - The fastest feature-rich C++11/14/17/20 single-header testing framework for unit tests and TDD http://bit.ly/doctest-docs
-   [doom-st](https://github.com/doom/strong_type) - C++ implementation of strong types
-   [EnTT](https://github.com/skypjack/entt) - Gaming meets modern C++ - a fast and reliable entity-component system (ECS).
-   [refl-cpp](https://github.com/veselink1/refl-cpp) A compile-time reflection library for modern C++ 🔥
-   [range-v3](https://github.com/ericniebler/range-v3) Range library for C++14/17/20, basis for C++20's std::ranges
-   [expected](https://github.com/TartanLlama/expected) C++11/14/17 std::expected with functional-style extensions https://tl.tartanllama.xyz
-   (optional)[SFML](https://github.com/SFML/SFML) Simple and Fast Multimedia Library http://www.sfml-dev.org/

## Code of Conduct

Before any contribution please read our [CODE OF CONDUCT](./CODE-OF-CONDUCT.md).

## Contributing

Please read [CONTRIBUTING.md](./CONTRIBUTING.md), contain the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/KomodoPlatform/antara-gaming-sdk/tags).

## Authors

-   **Roman Sztergbaum** - _Co-Creator_ & _Lead Dev_ - [Milerius](https://github.com/Milerius)
-   **Tolga Ay** - _Co-Creator_ - [naezith](https://github.com/Naezith)

See also the list of [contributors](./CONTRIBUTORS.md) who participated in this project.

## Acknowledgments

-  Viktor Kirilov [onqtam](https://github.com/onqtam) for the awesome `doctest` framework.
-  Michele Caini  [skypjack](https://github.com/skypjack) for the awesome `EnTT` framework and his help.

## Badges

| Apps                             | Badges                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
|----------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Github Actions CI (Windows/Osx/Linux)            | [![Build Status](https://github.com/KomodoPlatform/antara-gaming-sdk/workflows/CI/badge.svg)](https://github.com/KomodoPlatform/antara-gaming-sdk/actions) |
| License                          |  <a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/">Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License</a>.                                                                                                                                                                                                                                         |
| LGTM (Security C++) Alerts             | [![Total alerts](https://img.shields.io/lgtm/alerts/g/KomodoPlatform/antara-gaming-sdk.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/KomodoPlatform/antara-gaming-sdk/alerts/) |
| LGTM (Security C++) Quality             |  [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/KomodoPlatform/antara-gaming-sdk.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/KomodoPlatform/antara-gaming-sdk/context:cpp)                                                                                                                                                                                                                                               |
| Issues                           | ![GitHub issues](https://img.shields.io/github/issues/KomodoPlatform/antara-gaming-sdk)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                |
| Report CI (Linux/Osx/Windows/Emscripten)                | [![badge](https://report.ci/status/KomodoPlatform/antara-gaming-sdk/badge.svg?branch=master)](https://report.ci/status/KomodoPlatform/antara-gaming-sdk?branch=master)                                                             
| Coverage (Codecov)               | [![codecov](https://codecov.io/gh/KomodoPlatform/antara-gaming-sdk/branch/master/graph/badge.svg)](https://codecov.io/gh/KomodoPlatform/antara-gaming-sdk)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
| Docs                             |     [![Documentation Status](https://readthedocs.org/projects/antara-gaming-sdk/badge/?version=latest)](https://antara-gaming-sdk.readthedocs.io/en/latest/?badge=latest)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| HitCount                         | [![HitCount](http://hits.dwyl.io/KomodoPlatform/antara-gaming-sdk.svg)](http://hits.dwyl.io/KomodoPlatform/antara-gaming-sdk)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| Line Of Code                     | [![](https://tokei.rs/b1/github/KomodoPlatform/antara-gaming-sdk)](https://github.com/KomodoPlatform/antara-gaming-sdk)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                |
| Conventional Commit              | [![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-yellow.svg)](https://conventionalcommits.org)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |

