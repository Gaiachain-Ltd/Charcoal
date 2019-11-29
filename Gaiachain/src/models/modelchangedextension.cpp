#include "modelchangedextension.h"

ModelChangedExtension::ModelChangedExtension()
{}

ModelChangedExtension::~ModelChangedExtension()
{}

void ModelChangedExtension::setupConnections(const QObject *iObject)
{
    // new style connects doesn't work with qobject interfaces
    QObject::connect(iObject, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
                     iObject, SIGNAL(modelChanged()) );
    QObject::connect(iObject, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
                     iObject, SIGNAL(modelChanged()) );
    QObject::connect(iObject, SIGNAL(modelReset()),
                     iObject, SIGNAL(modelChanged()) );
}
