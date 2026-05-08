#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QFileSystemWatcher>

class LogViewer : public QWidget {
    Q_OBJECT
public:
    explicit LogViewer(QWidget *parent = nullptr);
    void loadLog(const QString &service);
private slots:
    void onClear();
    void onRefresh();
    void onSvcChanged(int idx);
    void onFileChanged(const QString &path);
private:
    void reloadCurrentLog();
    QComboBox          *m_svcCombo;
    QTextEdit          *m_logEdit;
    QFileSystemWatcher *m_watcher;
    QString             m_currentPath;
    QString             m_baseDir;
};
