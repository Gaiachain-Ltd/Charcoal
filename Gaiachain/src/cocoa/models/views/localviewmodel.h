#ifndef LOCALVIEWMODEL_H
#define LOCALVIEWMODEL_H

#include "cooperativeviewmodel.h"

class LocalViewModel final : public CooperativeViewModel
{
    Q_OBJECT

public:
    LocalViewModel(QObject *parent = nullptr);

private:
    void updateFilterQuery() override;
};

#endif // LOCALVIEWMODEL_H
