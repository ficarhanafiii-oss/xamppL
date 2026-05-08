#include "portchecker.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTcpSocket>
#include <QHeaderView>

PortChecker::PortChecker(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *lay = new QVBoxLayout(this);
    QPushButton *btn = new QPushButton("Run Port Check", this);
    btn->setStyleSheet("background:#c678dd;color:#1e2127;font-weight:bold;padding:6px 18px;border-radius:4px;");
    lay->addWidget(btn);
    m_table = new QTableWidget(0,4,this);
    m_table->setHorizontalHeaderLabels({"Service","Port","Status","Note"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    lay->addWidget(m_table);
    connect(btn, &QPushButton::clicked, this, &PortChecker::runCheck);
    runCheck();
}
bool PortChecker::isPortInUse(int port) {
    QTcpSocket s;
    s.connectToHost("127.0.0.1",port);
    bool r=s.waitForConnected(300);
    s.close(); return r;
}
void PortChecker::runCheck() {
    struct PD { QString svc; int port; };
    QList<PD> ports={
        {"Apache (HTTP)",80},{"Apache (HTTPS)",443},
        {"MariaDB",3306},{"FTP Control",21},
        {"FTP Passive",2121},{"SMTP",25},
        {"POP3",110},{"IMAP",143}
    };
    m_table->setRowCount(ports.size());
    for (int i=0;i<ports.size();++i) {
        bool used=isPortInUse(ports[i].port);
        m_table->setItem(i,0,new QTableWidgetItem(ports[i].svc));
        m_table->setItem(i,1,new QTableWidgetItem(QString::number(ports[i].port)));
        auto *st=new QTableWidgetItem(used?"● In Use":"○ Free");
        st->setForeground(used?QColor("#e06c75"):QColor("#98c379"));
        m_table->setItem(i,2,st);
        m_table->setItem(i,3,new QTableWidgetItem(used?"Conflict!":"OK"));
    }
}
