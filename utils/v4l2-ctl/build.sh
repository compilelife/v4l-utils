rm -rf build
mkdir build

ROOT_PWD=$(pwd)

cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$ROOT_PWD/rk3588.android.arm64v8a.cmake \
         -DCMAKE_ANDROID_NDK=/mnt/workspace/cy/devkit/android-ndk-r21b/ \
         -DCMAKE_BUILD_TYPE=Debug

make