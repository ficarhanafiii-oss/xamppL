#!/bin/bash
set -e
INSTALL_DIR="/opt/myxampp"
echo "=== MyXAMPP Installer for Linux ARM64 ==="
[ "$EUID" -ne 0 ] && { echo "Run as root: sudo bash install.sh"; exit 1; }
echo "[1/6] Installing runtime packages..."
apt-get update -qq
apt-get install -y apache2 mariadb-server php php-cli php-mysql \
    php-curl php-gd php-mbstring php-xml php-zip vsftpd \
    libqt6widgets6 libqt6network6
echo "[2/6] Creating directory structure..."
bash scripts/setup_dirs.sh
echo "[3/6] Installing configurations..."
cp -rn conf/apache/* $INSTALL_DIR/apache/conf/
cp -n  conf/mariadb/my.cnf $INSTALL_DIR/mariadb/config/
cp -n  conf/php/php.ini $INSTALL_DIR/php/lib/
cp -n  conf/ftp/vsftpd.conf $INSTALL_DIR/etc/
if [ -f build/myxampp ]; then
    echo "[4/6] Installing GUI binary..."
    cp build/myxampp $INSTALL_DIR/bin/
fi
cp myxamppctl $INSTALL_DIR/bin/
chmod +x $INSTALL_DIR/bin/myxamppctl
echo "[5/6] Initializing MariaDB..."
bash scripts/init_db.sh
echo "[6/6] Creating desktop entry..."
cat > /usr/share/applications/myxampp.desktop << DESK
[Desktop Entry]
Version=1.0
Type=Application
Name=MyXAMPP
Comment=XAMPP-like control panel for ARM64
Exec=$INSTALL_DIR/bin/myxampp
Terminal=false
Categories=Development;WebDevelopment;
DESK
echo ""
echo "✓ MyXAMPP installed to $INSTALL_DIR"
echo "GUI : /opt/myxampp/bin/myxampp"
echo "CLI : /opt/myxampp/bin/myxamppctl start"
