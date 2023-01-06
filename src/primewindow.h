#ifndef PRIMEWINDOW_H
#define PRIMEWINDOW_H

#include "configparse.h"
#include <QLineEdit>
#include <QMainWindow>

namespace Ui
{
class primewindow;
extern char **environ;
}

class primewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit primewindow(QWidget* parent = nullptr);
    ~primewindow();

    void run_tests();
    void init_env();
    void restore_defaults();
    void save_config();

private:
    Ui::primewindow* m_ui;
    prime::configparse m_config;
    std::vector<QLineEdit*> m_lines;
};

#endif // PRIMEWINDOW_H
