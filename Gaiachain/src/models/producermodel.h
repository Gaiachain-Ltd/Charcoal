#ifndef PRODUCERMODEL_H
#define PRODUCERMODEL_H

#include "abstractmodel.h"

#include <QHash>

class ProducerModel : public AbstractModel
{
    Q_OBJECT

public:
    enum ModelRole {
        ProducerId = Qt::UserRole,
        Name,
        Village,
        ParcelCodes,
        LastRole
    }; //!!! Add new roles at the end

    explicit ProducerModel(QObject *parent = nullptr);

    int lastRole() const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    static const QHash<int, QByteArray> sc_roleNames;
};

#endif // PRODUCERMODEL_H
