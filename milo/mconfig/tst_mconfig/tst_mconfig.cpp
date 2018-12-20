/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "mconfig.h"
#include "submconfig.h"

#ifdef _WIN32
  #include "windows.h"
#endif

class TestMConfig : public QObject
{
   Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testLoad1_data();
    void testLoad1();
    void testSave1_data();
    void testSave1();

    void testLoad2_data();
    void testLoad2();
    void testSave2_data();
    void testSave2();

#ifdef MCRYPTO_LIB
    void testSaveEncrypted1_data();
    void testSaveEncrypted1();
    void testLoadEncrypted1_data();
    void testLoadEncrypted1();

    void testLoadEncrypted2_data();
    void testLoadEncrypted2();
    void testSaveEncrypted2_data();
    void testSaveEncrypted2();
#endif


private:
    QString m_testConfigPath = QCoreApplication::applicationDirPath() + "/miloConfTest.ini";

    void refreshRows();
};

void TestMConfig::refreshRows()
{
    QTest::addColumn<QByteArray>("groupName");
    QTest::newRow("SimpleGroupName") << QByteArray("TestGroup");
    QTest::newRow("NumberGroupName") << QByteArray("123");
    QTest::newRow("Spaced GroupName") << QByteArray("Test Group");
}

void TestMConfig::initTestCase()
{
    QCoreApplication::setApplicationName("MConfig Test");
    QCoreApplication::setOrganizationName("Milo");
}

void TestMConfig::cleanupTestCase()
{
    MConfig config("Test");
    QFile::remove(config.filePath());
    QFile::remove(m_testConfigPath);
}

void TestMConfig::testLoad1_data()
{
    refreshRows();
}

void TestMConfig::testLoad1()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config1(groupName);
    config1.zeroData();
    config1.load(":/data/miloConfTest.ini");

    SubMConfig config2(groupName);

    QCOMPARE(config1, config2);
}

void TestMConfig::testSave1_data()
{
    refreshRows();
}

void TestMConfig::testSave1()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config(groupName);
    config.save();
#ifdef _WIN32
    // Checking if registry path exists (only possible using Windows API)
    HKEY hk;
    QString str = config.filePath().remove("\\HKEY_CURRENT_USER\\");
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, reinterpret_cast<LPCWSTR>(str.utf16()), 0, KEY_READ, &hk);
    QCOMPARE(result, ERROR_SUCCESS);
#else
    QVERIFY(QFile::exists(config.filePath()));
#endif
}

void TestMConfig::testLoad2_data()
{
    refreshRows();
}

void TestMConfig::testLoad2()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config1(groupName);
    SubMConfig config2(groupName);

    config1.zeroData();
    QVERIFY(!(config1 == config2));
    config1.load();
    QCOMPARE(config1, config2);
}

void TestMConfig::testSave2_data()
{
    refreshRows();
}

void TestMConfig::testSave2()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config(groupName);
    config.save(m_testConfigPath);
    QVERIFY(QFile::exists(m_testConfigPath));
}

#ifdef MCRYPTO_LIB
// Testing encrypted save/load

void TestMConfig::testSaveEncrypted1_data()
{
    refreshRows();
}

void TestMConfig::testSaveEncrypted1()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config(groupName);
    config.saveEncrypted();
#ifdef _WIN32
    // Checking if registry path exists (only possible using Windows API)
    HKEY hk;
    QString str = config.filePath().remove("\\HKEY_CURRENT_USER\\");
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, reinterpret_cast<LPCWSTR>(str.utf16()), 0, KEY_READ, &hk);
    QCOMPARE(result, ERROR_SUCCESS);
#else
    QVERIFY(QFile::exists(config.filePath()));
#endif
}

void TestMConfig::testLoadEncrypted1_data()
{
    refreshRows();
}

void TestMConfig::testLoadEncrypted1()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config1(groupName);
    config1.zeroData();
    config1.loadEncrypted();

    SubMConfig config2(groupName);

    QCOMPARE(config1, config2);
}

void TestMConfig::testLoadEncrypted2_data()
{
    refreshRows();
}

void TestMConfig::testLoadEncrypted2()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config1(groupName);
    SubMConfig config2(groupName);

    config1.zeroData();
    QVERIFY(!(config1 == config2));
    config1.loadEncrypted();
    QCOMPARE(config1, config2);
}

void TestMConfig::testSaveEncrypted2_data()
{
    QFile::remove(m_testConfigPath);
    refreshRows();
}

void TestMConfig::testSaveEncrypted2()
{
    QFETCH(QByteArray, groupName);
    SubMConfig config(groupName);
    config.saveEncrypted(m_testConfigPath);
    QVERIFY(QFile::exists(m_testConfigPath));
}
#endif

QTEST_MAIN(TestMConfig)

#include "tst_mconfig.moc"
