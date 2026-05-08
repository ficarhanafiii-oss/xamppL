#include "servicemanager.h"
#include <QFile>
#include <QTextStream>
#include <QTcpSocket>
#include <csignal>

ServiceManager::ServiceManager(QObject *parent) : QObject(parent) {
    m_baseDir = MYXAMPP_BASE_DIR;
    m_servicePorts = {{"Apache",80},{"MariaDB",3306},{"FTP",21}};
}

bool ServiceManager::isPortInUse(int port) {
    QTcpSocket s;
    s.connectToHost("127.0.0.1", port);
    bool used = s.waitForConnected(300);
    s.close();
    return used;
}

QString ServiceManager::pidFile(const QString &svc) {
    return m_baseDir + "/var/run/" + svc.toLower() + ".pid";
}
qint64 ServiceManager::readPid(const QString &svc) {
    QFile f(pidFile(svc));
    if (!f.open(QIODevice::ReadOnly)) return 0;
    return QTextStream(&f).readAll().trimmed().toLongLong();
}
void ServiceManager::writePid(const QString &svc, qint64 pid) {
    QFile f(pidFile(svc));
    if (f.open(QIODevice::WriteOnly)) QTextStream(&f) << pid;
}
void ServiceManager::clearPid(const QString &svc) { QFile::remove(pidFile(svc)); }

void ServiceManager::startService(const QString &name) {
    int port = m_servicePorts.value(name,0);
    if (port && isPortInUse(port)) return;
    QProcess *proc = m_processes.value(name, nullptr);
    if (!proc) { proc = new QProcess(this); m_processes[name] = proc; }
    if (name == "Apache") {
        proc->start("/usr/sbin/apache2",{"-f",m_baseDir+"/apache/conf/httpd.conf","-DFOREGROUND"});
    } else if (name == "MariaDB") {
        proc->start("/usr/sbin/mariadbd",{
            "--defaults-file="+m_baseDir+"/mariadb/config/my.cnf",
            "--datadir="+m_baseDir+"/mariadb/data",
            "--pid-file="+pidFile("MariaDB"),
            "--log-error="+m_baseDir+"/logs/mariadb_error.log"
        });
    } else if (name == "FTP") {
        proc->start("/usr/sbin/vsftpd",{m_baseDir+"/etc/vsftpd.conf"});
    }
    if (proc->waitForStarted(3000)) writePid(name, proc->processId());
}

void ServiceManager::stopService(const QString &name) {
    QProcess *proc = m_processes.value(name,nullptr);
    if (proc && proc->state()==QProcess::Running) {
        proc->terminate();
        if (!proc->waitForFinished(3000)) proc->kill();
    }
    qint64 pid = readPid(name);
    if (pid > 0) ::kill((pid_t)pid, SIGTERM);
    clearPid(name);
}

void ServiceManager::startAll() { for (auto &s:{"Apache","MariaDB","FTP"}) startService(s); }
void ServiceManager::stopAll()  { for (auto &s:{"Apache","MariaDB","FTP"}) stopService(s);  }

ServiceInfo ServiceManager::getServiceInfo(const QString &name) {
    ServiceInfo info;
    info.name    = name;
    info.port    = m_servicePorts.value(name,0);
    info.running = info.port > 0 && isPortInUse(info.port);
    if (info.running) info.pid = readPid(name);
    return info;
}
