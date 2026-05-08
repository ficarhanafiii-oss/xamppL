#!/bin/bash
echo "=== Checking MyXAMPP runtime dependencies ==="
DEPS=(apache2 mariadbd php vsftpd)
MISSING=()
for d in "${DEPS[@]}"; do
    if command -v $d &>/dev/null || [ -f /usr/sbin/$d ]; then
        echo "[OK]      $d"
    else
        MISSING+=($d); echo "[MISSING] $d"
    fi
done
if [ ${#MISSING[@]} -gt 0 ]; then
    echo ""
    echo "Install missing: sudo apt install ${MISSING[*]}"
    exit 1
fi
echo "All dependencies satisfied."
