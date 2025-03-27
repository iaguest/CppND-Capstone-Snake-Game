# build.ps1 — Clean and rebuild C++ project using CMake + Make (PowerShell)

$buildDir = Join-Path $PSScriptRoot "build"

# Clean: delete build folder if it exists
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
}

# Recreate build directory
New-Item -ItemType Directory -Path $buildDir | Out-Null

# Enter build directory
Push-Location $buildDir

# Configure and build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make

# Return to project root
Pop-Location
