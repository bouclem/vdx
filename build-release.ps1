# Build Release Script for VDX
# Builds the project, creates MSI installer, and copies to releases folder

$ErrorActionPreference = "Stop"

$rootDir = $PSScriptRoot
$vdxDir = Join-Path $rootDir "vdx"
$buildDir = Join-Path $vdxDir "build"
$releasesDir = Join-Path $rootDir "releases"

Write-Host "=== VDX Release Build Script ===" -ForegroundColor Cyan
Write-Host ""

# Step 1: Ensure build directory exists
Write-Host "[1/5] Setting up build directory..." -ForegroundColor Yellow
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
    Write-Host "      Created build directory" -ForegroundColor Green
} else {
    Write-Host "      Build directory exists" -ForegroundColor Green
}

# Step 2: Configure with CMake
Write-Host "[2/5] Configuring with CMake..." -ForegroundColor Yellow
Set-Location $vdxDir
$cmakeOutput = cmake -B build 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed!"
    exit 1
}
Write-Host "      CMake configured successfully" -ForegroundColor Green

# Step 3: Build the project
Write-Host "[3/5] Building project..." -ForegroundColor Yellow
$buildOutput = cmake --build build --config Release 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed!"
    exit 1
}
Write-Host "      Build completed successfully" -ForegroundColor Green

# Step 4: Create MSI installer with CPack
Write-Host "[4/5] Creating MSI installer..." -ForegroundColor Yellow
Set-Location $buildDir
$cpackOutput = cpack -C Release 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Error "MSI creation failed!"
    exit 1
}

# Find the generated MSI file
$msiFile = Get-ChildItem -Path $buildDir -Filter "*.msi" | Select-Object -First 1
if (-not $msiFile) {
    Write-Error "MSI file not found in build directory!"
    exit 1
}
Write-Host "      MSI created: $($msiFile.Name)" -ForegroundColor Green

# Step 5: Copy to releases folder
Write-Host "[5/5] Copying MSI to releases folder..." -ForegroundColor Yellow
if (-not (Test-Path $releasesDir)) {
    New-Item -ItemType Directory -Path $releasesDir -Force | Out-Null
    Write-Host "      Created releases directory" -ForegroundColor Green
}

$destination = Join-Path $releasesDir $msiFile.Name
Copy-Item -Path $msiFile.FullName -Destination $destination -Force
Write-Host "      MSI copied to: $destination" -ForegroundColor Green

# Summary
Write-Host ""
Write-Host "=== Build Complete ===" -ForegroundColor Cyan
Write-Host "MSI Location: $destination" -ForegroundColor White
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Test the installer: $destination" -ForegroundColor Gray
Write-Host "  2. Create GitHub release at: https://github.com/bouclem/vdx/releases/new" -ForegroundColor Gray
Write-Host "  3. Upload the MSI to the release" -ForegroundColor Gray
Write-Host ""

# Return to original directory
Set-Location $rootDir
