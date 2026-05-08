#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QLabel>

class ConfigEditor : public QWidget {
    Q_OBJECT
public:
    explicit ConfigEditor(QWidget *parent = nullptr);
    void loadConfig(const QString &service);
private slots:
    void onSave();
    void onReload();
    void onSvcChanged(int idx);
private:
    QComboBox      *m_svcCombo;
    QPlainTextEdit *m_editor;
    QLabel         *m_pathLabel;
    QString         m_currentPath;
    QString         m_baseDir;
};
