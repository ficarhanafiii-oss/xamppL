#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCloseEvent>
#include <QApplication>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_svcMgr      = new ServiceManager(this);
    m_logViewer   = new LogViewer(this);
    m_cfgEditor   = new ConfigEditor(this);
    m_portChecker = new PortChecker(this);
    setupUI();
    setupTray();
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshStatus);
    m_refreshTimer->start(2000);
    onRefreshStatus();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    setWindowTitle("MyXAMPP Control Panel v" MYXAMPP_VERSION);
    resize(900, 600);
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *header = new QLabel("  MyXAMPP Control Panel", this);
    QFont hf = header->font(); hf.setPointSize(16); hf.setBold(true);
    header->setFont(hf);
    header->setStyleSheet("color:#61afef; padding:8px;");
    mainLayout->addWidget(header);

    m_tabs = new QTabWidget(this);
    mainLayout->addWidget(m_tabs);

    // -- Services Tab --
    QWidget *svcTab = new QWidget();
    QVBoxLayout *svcLayout = new QVBoxLayout(svcTab);

    QHBoxLayout *btnRow = new QHBoxLayout();
    auto mkGBtn = [&](const QString &label, const QString &bg) {
        QPushButton *b = new QPushButton(label, this);
        b->setStyleSheet(QString("background:%1;color:#1e2127;font-weight:bold;padding:6px 18px;border-radius:4px;").arg(bg));
        return b;
    };
    QPushButton *btnStartAll = mkGBtn("Start All",  "#98c379");
    QPushButton *btnStopAll  = mkGBtn("Stop All",   "#e06c75");
    QPushButton *btnRefresh  = mkGBtn("Refresh",    "#61afef");
    QPushButton *btnPorts    = mkGBtn("Port Check", "#c678dd");
    btnRow->addWidget(btnStartAll); btnRow->addWidget(btnStopAll);
    btnRow->addWidget(btnRefresh);  btnRow->addWidget(btnPorts);
    btnRow->addStretch();
    svcLayout->addLayout(btnRow);

    connect(btnStartAll, &QPushButton::clicked, this, &MainWindow::onStartAll);
    connect(btnStopAll,  &QPushButton::clicked, this, &MainWindow::onStopAll);
    connect(btnRefresh,  &QPushButton::clicked, this, &MainWindow::onRefreshStatus);
    connect(btnPorts,    &QPushButton::clicked, this, &MainWindow::onCheckPorts);

    m_statusTable = new QTableWidget(0, 8, this);
    m_statusTable->setHorizontalHeaderLabels({"Service","Status","PID","Port","Start","Stop","Logs","Config"});
    m_statusTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_statusTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_statusTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_statusTable->setAlternatingRowColors(true);
    m_statusTable->verticalHeader()->setVisible(false);
    svcLayout->addWidget(m_statusTable);
    m_tabs->addTab(svcTab, "Services");
    m_tabs->addTab(m_logViewer,   "Log Viewer");
    m_tabs->addTab(m_cfgEditor,   "Config Editor");
    m_tabs->addTab(m_portChecker, "Port Checker");

    m_statusBar = new QLabel("Ready", this);
    mainLayout->addWidget(m_statusBar);

    const QStringList services = {"Apache","MariaDB","FTP"};
    m_statusTable->setRowCount(services.size());
    for (int i = 0; i < services.size(); ++i) {
        m_statusTable->setItem(i,0, new QTableWidgetItem(services[i]));
        m_statusTable->setItem(i,1, new QTableWidgetItem("Checking..."));
        m_statusTable->setItem(i,2, new QTableWidgetItem("-"));
        m_statusTable->setItem(i,3, new QTableWidgetItem("-"));
        QString svc = services[i];
        auto mkBtn = [&](const QString &lbl, const QString &bg) {
            QPushButton *b = new QPushButton(lbl);
            b->setStyleSheet(QString("background:%1;color:#1e2127;border-radius:3px;padding:3px 8px;").arg(bg));
            return b;
        };
        auto *startBtn = mkBtn("Start", "#98c379");
        auto *stopBtn  = mkBtn("Stop",  "#e06c75");
        auto *logBtn   = mkBtn("Logs",  "#61afef");
        auto *cfgBtn   = mkBtn("Config","#e5c07b");
        connect(startBtn,&QPushButton::clicked,[this,svc]{onServiceAction(svc,"start");});
        connect(stopBtn, &QPushButton::clicked,[this,svc]{onServiceAction(svc,"stop");});
        connect(logBtn,  &QPushButton::clicked,[this,svc]{onOpenLogs(svc);});
        connect(cfgBtn,  &QPushButton::clicked,[this,svc]{onOpenConfig(svc);});
        m_statusTable->setCellWidget(i,4,startBtn);
        m_statusTable->setCellWidget(i,5,stopBtn);
        m_statusTable->setCellWidget(i,6,logBtn);
        m_statusTable->setCellWidget(i,7,cfgBtn);
    }
}

void MainWindow::setupTray() {
    m_tray = new QSystemTrayIcon(this);
    m_tray->setToolTip("MyXAMPP");
    m_trayMenu = new QMenu();
    m_trayMenu->addAction("Show",      this,  &QWidget::show);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Start All", this,  &MainWindow::onStartAll);
    m_trayMenu->addAction("Stop All",  this,  &MainWindow::onStopAll);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Quit",      qApp,  &QCoreApplication::quit);
    m_tray->setContextMenu(m_trayMenu);
    m_tray->show();
    connect(m_tray,&QSystemTrayIcon::activated,this,&MainWindow::onTrayActivated);
}

void MainWindow::onStartAll()  { m_svcMgr->startAll();  onRefreshStatus(); }
void MainWindow::onStopAll()   { m_svcMgr->stopAll();   onRefreshStatus(); }
void MainWindow::onCheckPorts(){ m_portChecker->runCheck(); m_tabs->setCurrentWidget(m_portChecker); }

void MainWindow::onServiceAction(const QString &svc, const QString &action) {
    if (action=="start") m_svcMgr->startService(svc);
    else                 m_svcMgr->stopService(svc);
    QTimer::singleShot(800, this, &MainWindow::onRefreshStatus);
}
void MainWindow::onOpenLogs(const QString &svc)  { m_logViewer->loadLog(svc); m_tabs->setCurrentWidget(m_logViewer); }
void MainWindow::onOpenConfig(const QString &svc){ m_cfgEditor->loadConfig(svc); m_tabs->setCurrentWidget(m_cfgEditor); }

void MainWindow::onRefreshStatus() {
    const QStringList services = {"Apache","MariaDB","FTP"};
    for (int i = 0; i < services.size(); ++i) {
        ServiceInfo info = m_svcMgr->getServiceInfo(services[i]);
        if (auto *si = m_statusTable->item(i,1)) {
            si->setText(info.running ? "● RUNNING" : "○ STOPPED");
            si->setForeground(info.running ? QColor("#98c379") : QColor("#e06c75"));
        }
        if (auto *pi = m_statusTable->item(i,2)) pi->setText(info.running ? QString::number(info.pid) : "-");
        if (auto *poi= m_statusTable->item(i,3)) poi->setText(info.port>0 ? QString::number(info.port) : "-");
    }
    m_statusBar->setText("Last refresh: " + QDateTime::currentDateTime().toString("hh:mm:ss"));
}
void MainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason r) {
    if (r == QSystemTrayIcon::DoubleClick) show();
}
void MainWindow::closeEvent(QCloseEvent *event) {
    hide(); event->ignore();
    m_tray->showMessage("MyXAMPP","Running in background",QSystemTrayIcon::Information,2000);
}
