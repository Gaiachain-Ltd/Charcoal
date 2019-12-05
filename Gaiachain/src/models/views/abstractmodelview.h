#ifndef ABSTRACTMODELVIEW_H
#define ABSTRACTMODELVIEW_H

#include <QPointer>

#include "../proxy/sqlquerymodel.h"

class AbstractModelView
{
public:
    AbstractModelView();
    virtual ~AbstractModelView();

    virtual void setSourceModel(SqlQueryModel *sourceModel) = 0;

protected:
    QPointer<SqlQueryModel> m_queryModel;

    virtual void updateFilterQuery() = 0;
};

#endif // ABSTRACTMODELVIEW_H
