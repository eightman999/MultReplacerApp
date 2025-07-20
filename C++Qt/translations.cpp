#include "translations.h"
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>

Language Translations::currentLang = Language::JA;
QMap<QString, QString> Translations::strings;

void Translations::load(Language lang)
{
    currentLang = lang;
    QString code = (lang == Language::JA) ? "ja" : "en";
    QString filePath = QCoreApplication::applicationDirPath() + "/lang/" + code + ".json";
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();
        strings.clear();
        for (const QString& k : obj.keys()) {
            strings[k] = obj[k].toString();
        }
    }
}

QString Translations::tr(const QString& key)
{
    return strings.value(key, key);
}
