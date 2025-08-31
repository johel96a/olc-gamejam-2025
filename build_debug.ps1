# Debug build script

Set-Location -Path $PSScriptRoot

if (!(Test-Path -Path "./build")) {
    New-Item -ItemType Directory -Path "./build"
}

cmake -S . -B ./build
cmake --build ./build --config Debug

$sourceAssets = Join-Path $PSScriptRoot "assets"
$targetAssets = Join-Path $PSScriptRoot "build/Debug/assets"

if (!(Test-Path -Path $targetAssets)) {
    Copy-Item -Path $sourceAssets -Destination $targetAssets -Recurse
}

Start-Process -FilePath "./build/Debug/game.exe"
