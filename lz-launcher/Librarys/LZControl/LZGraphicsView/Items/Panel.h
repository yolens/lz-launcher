#ifndef PANEL_H
#define PANEL_H

#include "Node.h"

class ControlPanel : public Node
{
    Q_OBJECT
public:
    explicit ControlPanel(QObject *parent = nullptr, LCType type = LC_Panel);

signals:

};

#endif // PANEL_H
