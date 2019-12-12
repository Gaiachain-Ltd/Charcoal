#include "fakeserverstate.h"

FakeServerState &FakeServerState::instance()
{
    static FakeServerState serverState;
    return serverState;
}

void FakeServerState::upsideDown()
{
    setUp(!m_up);
}

bool FakeServerState::up() const
{
    return m_up;
}

void FakeServerState::setUp(bool up)
{
    if (m_up == up)
        return;

    m_up = up;
    emit upChanged(m_up);
}

FakeServerState::FakeServerState()
    : QObject()
{}
