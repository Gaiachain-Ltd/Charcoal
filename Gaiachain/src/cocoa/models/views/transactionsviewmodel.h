#ifndef TRANSACTIONSVIEWMODEL_H
#define TRANSACTIONSVIEWMODEL_H

#include "cooperativeviewmodel.h"

#include "common/enums.h"

class TransactionsViewModel final : public CooperativeViewModel
{
    Q_OBJECT
    Q_PROPERTY(QString keyword READ keyword WRITE setKeyword NOTIFY keywordChanged)

public:
    TransactionsViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void clear();

    Q_INVOKABLE bool isPackageTypeActive(const Enums::PackageType &type) const;
    Q_INVOKABLE QString keyword() const;

    Q_INVOKABLE QSet<Enums::PackageType> activePackageTypes() const;

public slots:
    void setPackageTypeActive(const Enums::PackageType &type, bool active);
    void setKeyword(const QString &keyword);

signals:
    void packageTypeActiveChanged(const Enums::PackageType &type, bool active) const;
    void keywordChanged(const QString &keyword) const;

private:
    void initialiseActivePackageTypes();

    QSet<Enums::PackageType> m_activePackageTypes;
    QString m_keyword;

    void updateFilterQuery() override;
};

#endif // TRANSACTIONSVIEWMODEL_H
