@pushd %~dp0

set PROJ_NAME=Violetland

@if "%1"=="/ndkonly" goto ndkonly
@if "%1"=="/sdkonly" goto sdkonly

:ndkonly
@rem Compile JNI
@rem call bash -c "ndk-build NDK_DEBUG=0 V=0 BUILD_TYPE=release"
call ndk-build.cmd NDK_DEBUG=0 V=0 BUILD_TYPE=release
@if ERRORLEVEL 1 popd & exit 1
@if "%1"=="/ndkonly" popd & exit

:sdkonly
@rem Compile Java and create APK
@rem Unsigned APK on output
call ant release
@if ERRORLEVEL 1 popd & exit 1

@popd
