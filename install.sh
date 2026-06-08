#!/usr/bin/env bash

set -e

# Configuration
REPO="arnauda-gh/Alwide"
INSTALL_DIR="$HOME/.local/bin"
CONFIG_DIR="$HOME/.config/alwide"
TMP_DIR=$(mktemp -d)

echo "🚀 Installing Alwide..."

# Check dependencies
if ! command -v curl &> /dev/null; then
    echo "❌ Error: curl is not installed."
    exit 1
fi

if ! command -v tar &> /dev/null; then
    echo "❌ Error: tar is not installed."
    exit 1
fi

# Get latest release info
RELEASE_INFO=$(curl -s "https://api.github.com/repos/$REPO/releases/latest")
BINARY_URL=$(echo "$RELEASE_INFO" | grep "browser_download_url" | grep "linux-x86_64.tar.gz" | cut -d '"' -f 4)
ASSETS_URL=$(echo "$RELEASE_INFO" | grep "browser_download_url" | grep "alwide-assets.zip" | cut -d '"' -f 4)

if [ -z "$BINARY_URL" ] || [ -z "$ASSETS_URL" ]; then
    echo "❌ Error: Could not find release assets. Please check if a release exists on GitHub."
    exit 1
fi

# Create directories
mkdir -p "$INSTALL_DIR"
mkdir -p "$CONFIG_DIR"

# Download and install binary
echo "📥 Downloading binary..."
curl -L "$BINARY_URL" -o "$TMP_DIR/alwide.tar.gz"
tar -xzf "$TMP_DIR/alwide.tar.gz" -C "$TMP_DIR"
mv "$TMP_DIR/al" "$INSTALL_DIR/al"
chmod +x "$INSTALL_DIR/al"

# Download and install assets
echo "📥 Downloading assets..."
curl -L "$ASSETS_URL" -o "$TMP_DIR/assets.zip"
unzip -o "$TMP_DIR/assets.zip" -d "$TMP_DIR"
cp -r "$TMP_DIR/assets/"* "$CONFIG_DIR/"

# Cleanup
rm -rf "$TMP_DIR"

echo "✅ Alwide installed successfully!"
echo "📍 Binary: $INSTALL_DIR/al"
echo "📍 Config & Assets: $CONFIG_DIR"
echo ""
echo "Make sure $INSTALL_DIR is in your PATH."
echo "You can now run 'al' to start the editor."
