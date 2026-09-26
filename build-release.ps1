# Build Release Script for VDX
# Builds the project, creates MSI installer, and copies to releases folder

$ErrorActionPreference = "Stop"

$rootDir = $PSScriptRoot
$vdxDir = Join-Path $rootDir "vdx"
$buildDir = Join-Path $vdxDir "build"
$releasesDir = Join-Path $rootDir "releases"

function Fail([string]$message, [array]$output) {
    if ($output) {
        $output | Select-Object -Last 40 | ForEach-Object { Write-Host $_ }
    }
    throw $message
}

Write-Host "=== VDX Release Build Script ===" -ForegroundColor Cyan
Write-Host ""

# Step 0: Check prerequisites
Write-Host "[0/5] Checking prerequisites..." -ForegroundColor Yellow
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Fail "cmake not found on PATH" @()
}
if (-not (Get-Command wix -ErrorAction SilentlyContinue)) {
    Fail "WiX 4 toolset ('wix') not found on PATH - required for MSI packaging" @()
}
Write-Host "      cmake and wix found" -ForegroundColor Green

# Step 1: Clean and create build directory
Write-Host "[1/5] Setting up build directory..." -ForegroundColor Yellow
if (Test-Path $buildDir) {
    Remove-Item -Path $buildDir -Recurse -Force
}
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
Write-Host "      Build directory prepared" -ForegroundColor Green

# Step 2: Configure with CMake
Write-Host "[2/5] Configuring with CMake..." -ForegroundColor Yellow
$cmakeOutput = cmake -S $vdxDir -B $buildDir 2>&1
if ($LASTEXITCODE -ne 0) {
    Fail "CMake configuration failed!" $cmakeOutput
}
Write-Host "      CMake configured successfully" -ForegroundColor Green

# Step 3: Build the project
Write-Host "[3/5] Building project..." -ForegroundColor Yellow
$buildOutput = cmake --build $buildDir --config Release 2>&1
if ($LASTEXITCODE -ne 0) {
    Fail "Build failed!" $buildOutput
}
Write-Host "      Build completed successfully" -ForegroundColor Green

# Step 4: Create MSI installer with CPack
Write-Host "[4/5] Creating MSI installer..." -ForegroundColor Yellow
Push-Location $buildDir
try {
    $cpackOutput = cpack -C Release 2>&1
    $cpackExit = $LASTEXITCODE
} finally {
    Pop-Location
}
if ($cpackExit -ne 0) {
    Fail "MSI creation failed!" $cpackOutput
}

# Find the generated MSI file
$msiFile = Get-ChildItem -Path $buildDir -Filter "*.msi" | Select-Object -First 1
if (-not $msiFile) {
    Fail "MSI file not found in build directory!" @()
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
