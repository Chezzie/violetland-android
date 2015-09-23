@pushd %~dp0

@if "%1"=="/ndkonly" goto ndkonly
@if "%1"=="/sdkonly" goto sdkonly

:ndkonly
@rem Compile JNI
@rem call bash -c "ndk-build NDK_DEBUG=1 V=0 BUILD_TYPE=debug"
call ndk-build.cmd NDK_DEBUG=1 V=0 BUILD_TYPE=debug
@if ERRORLEVEL 1 popd & exit 1
@if "%1"=="/ndkonly" popd & exit

:sdkonly
@rem Compile Java and create debug APK
call ant debug

@popd
