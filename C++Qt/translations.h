#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H
#include <QString>
#include <QMap>

enum class Language { JA, EN };

class Translations {
public:
    static void load(Language lang);
    static QString tr(const QString& key);
    static Language currentLang;
private:
    static QMap<QString, QString> strings;
};

#endif // TRANSLATIONS_H
