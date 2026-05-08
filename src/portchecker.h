#pragma once
#include <QWidget>
#include <QTableWidget>

class PortChecker : public QWidget {
    Q_OBJECT
public:
    explicit PortChecker(QWidget *parent = nullptr);
    void runCheck();
private:
    QTableWidget *m_table;
    bool isPortInUse(int port);
};
