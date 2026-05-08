#include "ftpserver.h"
#include <QFile>
FtpServer::FtpServer(QObject *parent) : QObject(parent) {}
bool FtpServer::validateConfig(const QString &p) {
    QFile f(p); return f.exists() && f.open(QIODevice::ReadOnly);
}
QString FtpServer::defaultConfig() {
    return "listen=YES\nanonymous_enable=NO\nlocal_enable=YES\n"
           "write_enable=YES\nlocal_umask=022\ndirmessage_enable=YES\n"
           "use_localtime=YES\nxferlog_enable=YES\nconnect_from_port_20=YES\n"
           "chroot_local_user=YES\nallow_writeable_chroot=YES\n"
           "secure_chroot_dir=/var/run/vsftpd/empty\npam_service_name=vsftpd\n"
           "pasv_enable=YES\npasv_min_port=10000\npasv_max_port=10100\n";
}
