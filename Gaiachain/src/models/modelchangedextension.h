#ifndef MODELCHANGEDEXTENSION_H
#define MODELCHANGEDEXTENSION_H

#include <QObject>
#include <QAbstractItemModel>

class ModelChangedExtension
{
public:
    ModelChangedExtension();
    virtual ~ModelChangedExtension();

signals:
    virtual void modelChanged() const = 0;

protected:
    void setupConnections(const QObject *iObject);
};

Q_DECLARE_INTERFACE(ModelChangedExtension, "ModelChangedExtension")

#endif // MODELCHANGEDEXTENSION_H
