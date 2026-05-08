#pragma once
#include <QObject>
#include <QString>
#include <QProcess>
#include <QMap>

struct ServiceInfo {
    QString name;
    bool    running = false;
    qint64  pid     = 0;
    int     port    = 0;
};

class ServiceManager : public QObject {
    Q_OBJECT
public:
    explicit ServiceManager(QObject *parent = nullptr);
    void startAll();
    void stopAll();
    void startService(const QString &name);
    void stopService(const QString &name);
    ServiceInfo getServiceInfo(const QString &name);
    bool isPortInUse(int port);
private:
    QString m_baseDir;
    QMap<QString,QProcess*> m_processes;
    QMap<QString,int>       m_servicePorts;
    QString pidFile(const QString &svc);
    qint64  readPid(const QString &svc);
    void    writePid(const QString &svc, qint64 pid);
    void    clearPid(const QString &svc);
};
