#ifndef MODBUSWINDOW_H
#define MODBUSWINDOW_H

#include <QMainWindow>

namespace Ui {
class ModbusWindow;
}

class ModbusWindow : public QMainWindow
{
    Q_OBJECT

private:
    explicit ModbusWindow(QWidget *parent = nullptr);
    ~ModbusWindow();
public:
    static ModbusWindow* instance();

    void init();
private:
    Ui::ModbusWindow *ui;
};

#endif // MODBUSWINDOW_H
