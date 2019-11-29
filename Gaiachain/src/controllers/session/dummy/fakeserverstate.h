#ifndef FAKESERVERSTATE_H
#define FAKESERVERSTATE_H

#include <QObject>

class FakeServerState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool up READ up WRITE setUp NOTIFY upChanged)

public:
    static FakeServerState &instance();

    Q_INVOKABLE void upsideDown();

    bool up() const;

public slots:
    void setUp(bool up);

signals:
    void upChanged(bool up);

private:
    FakeServerState();

    bool m_up = true;
};

#endif // FAKESERVERSTATE_H
