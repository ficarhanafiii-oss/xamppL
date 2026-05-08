#!/bin/bash
BASE="/opt/myxampp"
if [ ! -d "$BASE/mariadb/data/mysql" ]; then
    echo "Initializing MariaDB data directory..."
    mariadb-install-db \
        --user=mysql \
        --datadir=$BASE/mariadb/data \
        --skip-test-db
    echo "MariaDB initialized."
else
    echo "MariaDB data directory already exists."
fi
