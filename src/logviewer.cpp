#include "logviewer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>

LogViewer::LogViewer(QWidget *parent) : QWidget(parent) {
    m_baseDir = MYXAMPP_BASE_DIR;
    m_watcher = new QFileSystemWatcher(this);
    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout *top = new QHBoxLayout();
    m_svcCombo = new QComboBox(this);
    m_svcCombo->addItems({"Apache Access","Apache Error","MariaDB Error","FTP","MyXAMPP"});
    QPushButton *refreshBtn = new QPushButton("Refresh",    this);
    QPushButton *clearBtn   = new QPushButton("Clear View", this);
    top->addWidget(m_svcCombo); top->addWidget(refreshBtn);
    top->addWidget(clearBtn);   top->addStretch();
    lay->addLayout(top);
    m_logEdit = new QTextEdit(this);
    m_logEdit->setReadOnly(true);
    m_logEdit->setFont(QFont("Monospace",9));
    m_logEdit->setStyleSheet("background:#1e2127;color:#abb2bf;");
    lay->addWidget(m_logEdit);
    connect(m_svcCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LogViewer::onSvcChanged);
    connect(refreshBtn, &QPushButton::clicked, this, &LogViewer::onRefresh);
    connect(clearBtn,   &QPushButton::clicked, this, &LogViewer::onClear);
    connect(m_watcher,  &QFileSystemWatcher::fileChanged, this, &LogViewer::onFileChanged);
    onSvcChanged(0);
}

void LogViewer::loadLog(const QString &service) {
    QMap<QString,int> m={{"Apache",0},{"MariaDB",2},{"FTP",3}};
    if (m.contains(service)) m_svcCombo->setCurrentIndex(m[service]);
}
void LogViewer::onSvcChanged(int) {
    QStringList paths={
        m_baseDir+"/logs/apache_access.log",
        m_baseDir+"/logs/apache_error.log",
        m_baseDir+"/logs/mariadb_error.log",
        m_baseDir+"/logs/vsftpd.log",
        m_baseDir+"/logs/myxampp.log"
    };
    m_currentPath = paths.value(m_svcCombo->currentIndex());
    if (!m_watcher->files().isEmpty()) m_watcher->removePaths(m_watcher->files());
    m_watcher->addPath(m_currentPath);
    reloadCurrentLog();
}
void LogViewer::reloadCurrentLog() {
    QFile f(m_currentPath);
    if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) {
        m_logEdit->setPlainText("Log file not found:\n" + m_currentPath); return;
    }
    QStringList lines = QTextStream(&f).readAll().split('\n');
    if (lines.size()>500) lines = lines.mid(lines.size()-500);
    m_logEdit->setPlainText(lines.join('\n'));
    m_logEdit->verticalScrollBar()->setValue(m_logEdit->verticalScrollBar()->maximum());
}
void LogViewer::onRefresh()             { reloadCurrentLog(); }
void LogViewer::onClear()              { m_logEdit->clear(); }
void LogViewer::onFileChanged(const QString &) { reloadCurrentLog(); }
