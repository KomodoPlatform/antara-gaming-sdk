#!/usr/bin/env bash

function last_cmake_linux() {
          DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
          # shellcheck disable=SC2164
          mkdir ${DEPS_DIR} && cd ${DEPS_DIR}
          CMAKE_URL="https://cmake.org/files/v3.14/cmake-3.14.3-Linux-x86_64.tar.gz"
          mkdir cmake && travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
          export PATH=${DEPS_DIR}/cmake/bin:${PATH}
          # shellcheck disable=SC2164
          cd -
}

function coverage_setup() {
    if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
       sudo update-alternatives --install /usr/bin/gcov gcov /usr/bin/${COVERAGE_TOOL} 100
       cd /tmp
       curl -sL https://github.com/linux-test-project/lcov/releases/download/v1.14/lcov-1.14.tar.gz | tar xz
       sudo make -C lcov-1.14 install
       cd -
    elif [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then
       brew install lcov
    fi
}

function setup_osx() {
    export PATH="/usr/local/opt/llvm/bin:$PATH"
    sudo installer -pkg /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg -target /
}

function install_emscripten() {
    cd ${HOME}
    pwd
    git clone https://github.com/emscripten-core/emsdk.git
    cd -
    pwd
    cd ${HOME}/emsdk
    pwd
    git pull
    ./emsdk install latest-upstream
    ./emsdk activate latest-upstream
    source ./emsdk_env.sh
    cd -
    pwd
    cd ${HOME}
    pwd
    git clone https://github.com/emscripten-core/emscripten.git
    cd -
    pwd
    cd ${TRAVIS_BUILD_DIR}
    pwd
}

#if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then setup_osx || travis_terminate 1; fi
if [[ "${EMSCRIPTEN_WEB}" == "ON" ]]; then install_emscripten; fi
if [[ "${WILL_COMPILE_CODE}" == "ON" ]] && [[ "${TRAVIS_OS_NAME}" == "linux" ]] && [[ "${DEFAULT_COMPILER}" == "gcc" ]] ; then sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/${CXX} 100; fi
if [[ "${TRAVIS_OS_NAME}" == "linux" ]] && [[ "${WILL_COMPILE_CODE}" == "ON" ]]; then last_cmake_linux; fi
if [[ -n "${CODE_COVERAGE}" ]]; then coverage_setup || travis_terminate 1; fi