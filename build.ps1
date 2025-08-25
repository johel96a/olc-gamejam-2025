# build.ps1

# Optional: Change to project root
Set-Location -Path $PSScriptRoot

# Create build directory if missing
if (!(Test-Path -Path "./build")) {
    New-Item -ItemType Directory -Path "./build"
}

# Configure with CMake
cmake -S . -B ./build

# Build the project
cmake --build ./build --config Debug

# Run the executable (adjust name/path if needed)
# Add -NoNewWindow to remove debug console.
Start-Process -FilePath "./build/Debug/game.exe"
