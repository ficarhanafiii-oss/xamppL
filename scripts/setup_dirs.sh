#!/bin/bash
BASE="/opt/myxampp"
mkdir -p \
    $BASE/bin $BASE/etc $BASE/tmp $BASE/var/run \
    $BASE/logs $BASE/htdocs \
    $BASE/apache/conf/extra $BASE/apache/modules \
    $BASE/mariadb/bin $BASE/mariadb/data $BASE/mariadb/config \
    $BASE/php/bin $BASE/php/lib $BASE/php/ext \
    $BASE/ftp/root
cat > $BASE/htdocs/index.php << 'PHPEOF'
<?php
echo "<h1>MyXAMPP is Working on ARM64!</h1>";
echo "<p>PHP " . PHP_VERSION . "</p>";
phpinfo();
PHPEOF
chmod -R 755 $BASE
chown -R www-data:www-data $BASE/htdocs $BASE/logs 2>/dev/null || true
echo "Directories created at $BASE"
