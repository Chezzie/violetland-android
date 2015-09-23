@pushd %~dp0

set PROJ_NAME=Violetland

copy /Y /B AndroidManifest.xml AndroidManifest.xml.bak
copy /Y /B AndroidManifest-SlideME.xml AndroidManifest.xml
echo yes>assets\slideme

@if "%1"=="/ndkonly" goto ndkonly
@if "%1"=="/sdkonly" goto sdkonly

:ndkonly
@rem Compile JNI
@rem call bash -c "ndk-build NDK_DEBUG=0 V=0 BUILD_TYPE=release"
call ndk-build.cmd NDK_DEBUG=0 V=0 BUILD_TYPE=release
@if ERRORLEVEL 1 goto end
@if "%1"=="/ndkonly" goto end

:sdkonly
@rem Compile Java and create APK
@rem Unsigned APK on output
call ant release
@if ERRORLEVEL 1 goto end

:end
move /Y AndroidManifest.xml.bak AndroidManifest.xml
del /Q assets\slideme
@popd
