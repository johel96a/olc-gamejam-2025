# Release build script (no console window)

Set-Location -Path $PSScriptRoot

if (!(Test-Path -Path "./build")) {
    New-Item -ItemType Directory -Path "./build"
}

cmake -S . -B ./build
cmake --build ./build --config Release

$sourceAssets = Join-Path $PSScriptRoot "assets"
$targetAssets = Join-Path $PSScriptRoot "build/Release/assets"

if (!(Test-Path -Path $targetAssets)) {
    Copy-Item -Path $sourceAssets -Destination $targetAssets -Recurse
}

# -NoNewWindow to prevent console popup on Release
Start-Process -FilePath "./build/Release/game.exe" -NoNewWindow
