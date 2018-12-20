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

#ifndef MCONFIG_H
#define MCONFIG_H

#include <QByteArray>
#include <QHash>
#include <QMetaType>
#include <QSettings>
#include <QString>

#define CONFIG_VALUE(name, type) mValues.insert(#name, ValuePtr(type, static_cast<void *>(&name)));

class MConfig
{
 public:
    MConfig(const QByteArray &groupName);
    void load();
    void load(const QString &fileName, const QSettings::Format &format = QSettings::IniFormat);
    void save();
    void save(const QString &fileName, const QSettings::Format &format = QSettings::IniFormat);

#ifdef MCRYPTO_LIB
    void loadEncrypted();
    void loadEncrypted(const QString &fileName,
                       const QSettings::Format &format = QSettings::IniFormat);
    void saveEncrypted();
    void saveEncrypted(const QString &fileName,
                       const QSettings::Format &format = QSettings::IniFormat);
    //TODO we can think of simpler interface
    //if class is configured with mcrypto we need only to set passphrase
    //and the rest can be done in private load/save methods: if passphrase is empty
    //data are stored in plain mode if there is any passphrase set key/value will be encrypted
    void setPassphrase(const QByteArray &pass);
#endif

    QString filePath() const;

 protected:
    class ValuePtr
    {
     public:
        ValuePtr() {}
        ValuePtr(int t, void *v) : type(t), ptr(v) {}
        int type = QMetaType::UnknownType;
        void *ptr = nullptr;
    };
    QHash<QByteArray, ValuePtr> mValues;

 private:
    void load(QSettings& s);
    void save(QSettings& s);
    const QByteArray mGroupName;
    static void copyValue(void *dst, int type, const QVariant &value);

#ifdef MCRYPTO_LIB
    QByteArray mPassphrase;
#endif
};
#endif  // MCONFIG_H
