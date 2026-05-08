#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include "servicemanager.h"
#include "logviewer.h"
#include "configeditor.h"
#include "portchecker.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void onStartAll();
    void onStopAll();
    void onRefreshStatus();
    void onServiceAction(const QString &svc, const QString &action);
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void onOpenLogs(const QString &svc);
    void onOpenConfig(const QString &svc);
    void onCheckPorts();
private:
    void setupUI();
    void setupTray();
    QTabWidget      *m_tabs;
    QTableWidget    *m_statusTable;
    QTimer          *m_refreshTimer;
    QSystemTrayIcon *m_tray;
    QMenu           *m_trayMenu;
    ServiceManager  *m_svcMgr;
    LogViewer       *m_logViewer;
    ConfigEditor    *m_cfgEditor;
    PortChecker     *m_portChecker;
    QLabel          *m_statusBar;
};
