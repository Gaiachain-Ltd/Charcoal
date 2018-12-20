#include "testrequest.h"

TestRequest::TestRequest(const QUrl &url) : MRestRequest(url)
{
    mType = MRestRequest::Type::Get;
    mPriority = Priority::High;
}

// Intentionally empty
void TestRequest::parse()
{
    return;
}
