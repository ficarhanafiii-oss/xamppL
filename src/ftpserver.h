#pragma once
#include <QObject>
#include <QString>
class FtpServer : public QObject {
    Q_OBJECT
public:
    explicit FtpServer(QObject *parent = nullptr);
    bool    validateConfig(const QString &configPath);
    QString defaultConfig();
};
