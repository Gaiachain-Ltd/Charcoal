#include "ovensmodel.h"

OvensModel::OvensModel(QObject *parent) : QueryModel(parent)
{
}

void OvensModel::setPlotId(const QString &id)
{
    m_plotId = id;

    setDbQuery(QString("SELECT name FROM Ovens WHERE plot=%1").arg(m_plotId));
}

QString OvensModel::plotId() const
{
    return m_plotId;
}
