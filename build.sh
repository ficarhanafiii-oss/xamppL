#!/bin/bash
set -e
echo "=== MyXAMPP Build Script for ARM64 Debian ==="
ARCH=$(uname -m)
[ "$ARCH" != "aarch64" ] && echo "WARNING: Building on $ARCH, target is aarch64"
echo "[1/5] Installing build dependencies..."
sudo apt-get update -qq
sudo apt-get install -y build-essential cmake ninja-build \
    qt6-base-dev qt6-base-dev-tools libqt6widgets6 libqt6network6 libqt6dbus6 \
    qt6-tools-dev qt6-tools-dev-tools pkg-config libssl-dev
echo "[2/5] Configuring CMake..."
mkdir -p build && cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/myxampp
echo "[3/5] Building..."
ninja -j$(nproc)
echo "[4/5] Build complete!"
echo "Binary: build/myxampp"
echo "To install: sudo ninja install"
echo "Or run: sudo bash install.sh"
