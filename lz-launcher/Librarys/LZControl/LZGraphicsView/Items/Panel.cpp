#include "Panel.h"

ControlPanel::ControlPanel(QObject *parent, LCType type)
    : Node{parent, type}
{
    m_typeName = "Panel";
    this->setAcceptDrops(true);
}
