#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H
#include "IPlugin.h"

#define MAINWINDOW_UUID "{243c31c1-2d9b-56cb-adac-def801894f8e}"

class IMainWindow
{
public:
    virtual void showWindow(const bool show) = 0;
};
Q_DECLARE_INTERFACE(IMainWindow,"LZ.IMainWindow/1.0")

#endif // IMAINWINDOW_H
