#include "configeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

ConfigEditor::ConfigEditor(QWidget *parent) : QWidget(parent) {
    m_baseDir = MYXAMPP_BASE_DIR;
    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout *top = new QHBoxLayout();
    m_svcCombo = new QComboBox(this);
    m_svcCombo->addItems({"Apache (httpd.conf)","MariaDB (my.cnf)","PHP (php.ini)","FTP (vsftpd.conf)"});
    QPushButton *reloadBtn = new QPushButton("Reload", this);
    QPushButton *saveBtn   = new QPushButton("Save",   this);
    saveBtn->setStyleSheet("background:#98c379;color:#1e2127;font-weight:bold;padding:4px 14px;border-radius:3px;");
    top->addWidget(m_svcCombo); top->addWidget(reloadBtn); top->addWidget(saveBtn); top->addStretch();
    lay->addLayout(top);
    m_pathLabel = new QLabel(this);
    m_pathLabel->setStyleSheet("color:#5c6370;font-size:10px;");
    lay->addWidget(m_pathLabel);
    m_editor = new QPlainTextEdit(this);
    m_editor->setFont(QFont("Monospace",9));
    m_editor->setStyleSheet("background:#1e2127;color:#abb2bf;");
    lay->addWidget(m_editor);
    connect(m_svcCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConfigEditor::onSvcChanged);
    connect(reloadBtn,  &QPushButton::clicked, this, &ConfigEditor::onReload);
    connect(saveBtn,    &QPushButton::clicked, this, &ConfigEditor::onSave);
    onSvcChanged(0);
}
void ConfigEditor::loadConfig(const QString &service) {
    QMap<QString,int> m={{"Apache",0},{"MariaDB",1},{"PHP",2},{"FTP",3}};
    if (m.contains(service)) m_svcCombo->setCurrentIndex(m[service]);
}
void ConfigEditor::onSvcChanged(int) {
    QStringList paths={
        m_baseDir+"/apache/conf/httpd.conf",
        m_baseDir+"/mariadb/config/my.cnf",
        m_baseDir+"/php/lib/php.ini",
        m_baseDir+"/etc/vsftpd.conf"
    };
    m_currentPath = paths.value(m_svcCombo->currentIndex());
    m_pathLabel->setText(m_currentPath);
    onReload();
}
void ConfigEditor::onReload() {
    QFile f(m_currentPath);
    if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) {
        m_editor->setPlainText("# Config not found:\n# "+m_currentPath); return;
    }
    m_editor->setPlainText(QTextStream(&f).readAll());
}
void ConfigEditor::onSave() {
    QFile f(m_currentPath);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::critical(this,"Error","Cannot write:\n"+m_currentPath); return;
    }
    QTextStream(&f) << m_editor->toPlainText();
    QMessageBox::information(this,"Saved","Config saved.\nRestart service to apply.");
}
