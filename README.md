# fdx-natives

`fdx-natives` builds static third-party native dependencies used by libFDX
runtime `fdx` artifacts. The first package set contains FreeType and Tint/Dawn
for the platforms where libFDX packages the Tint-backed runtime shader compiler.

The project is intentionally a build-time helper. libFDX still produces the
final `fdx` runtime outputs (`.dll`, `.so`, `.dylib`, `.js`, `.wasm`, and
Android AAR contents), and users should not need to depend on `fdx-natives`
directly.

## Configuration

All dependency and toolchain pins live in `fdx-natives.toml`.

Current keys:

- `[package].version`
- `[freetype].version`, `[freetype].sha256`, `[freetype].urls`
- `[dawn].repository`, `[dawn].revision`
- `[android].ndkVersion`, `[android].minSdk`, `[android].cmakeVersion`
- `[web].emscriptenVersion`

`fdx-natives-lock.json` mirrors those pins for quick review and release notes.

## Common Tasks

```bash
./gradlew printNativeDepsConfig
./gradlew resolveFreetypeSource
./gradlew resolveDawnSource
./gradlew buildLinuxX64
./gradlew buildWindowsX64
./gradlew buildMacosX64
./gradlew buildMacosArm64
./gradlew buildAndroidAll
./gradlew buildWeb
./gradlew packageAll
./gradlew verifyPackages
./gradlew writeReleaseManifest
```

Build and package tasks only run when the current host/toolchain can support the
target. Android packaging requires `ANDROID_NDK_HOME`, or `ANDROID_HOME` /
`ANDROID_SDK_ROOT` with the configured NDK installed. Web packaging requires
`emcmake` on `PATH`.

## Updating Pins

```bash
./gradlew updateFreetype -PfdxNatives.freetypeVersion=2.14.3
./gradlew updateDawn -PfdxNatives.dawnRevision=<commit>
./gradlew updateToolchainPin -PfdxNatives.androidNdkVersion=27.0.12077973
./gradlew updateToolchainPin -PfdxNatives.emscriptenVersion=6.0.0
```

`updateFreetype` downloads the requested archive from the configured URL
templates, computes the SHA-256, and updates `fdx-natives.toml`.
`updateDawn` verifies the requested revision can be fetched before updating the
pin.

## Package Shape

Each ZIP contains importable CMake targets:

- `fdx_natives::freetype`
- `fdx_natives::tint`
- `fdx_natives::tint_api_helpers`
- `fdx_natives::tint_lang_core_ir_transform`
- `fdx_natives::tint_lang_glsl_writer_helpers`

Desktop and web packages have a direct package root:

```text
lib/
include/
cmake/fdx-natives-targets.cmake
```

The Android package contains one package root per ABI:

```text
android/arm64-v8a/lib/
android/arm64-v8a/include/
android/arm64-v8a/cmake/fdx-natives-targets.cmake
android/armeabi-v7a/...
android/x86/...
android/x86_64/...
```

`writeReleaseManifest` writes `build/packages/fdx-natives-manifest.json` with
package checksums and toolchain metadata.
