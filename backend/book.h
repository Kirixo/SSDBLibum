#ifndef BOOK_H
#define BOOK_H
#include <QObject>
#include <QUrl>
#include "genre.h"
#include "jsonable.h"

class Book : Jsonable
{
public:
    struct Property : Jsonable {
        qint64 id;
        QString name;

        Property() = default;
        Property(qint64 id, QString name);

        // Jsonable interface
        QJsonObject toJson() const override;
    };

    Book();
    ~Book();

    bool exists();

    void setId(qint64 newId);
    void setTitle(const QString &newTitle);
    void setCover(const QString &newCover);
    void setMeanScore(float newMeanScore);
    void setDescription(const QString &newDescription);
    void setGenres(const QList<Genre> &newGenres);
    void setLanguage(const Property &newLanguage);
    void setLanguageName(const QString &newLanguage);
    void setYear(qint16 newYear);
    void setAuthor(const Property &newAuthor);
    void setAuthorName(const QString &newAuthor);
    void setPrice(float newPrice);

    qint64 id() const;
    const QString& title() const;
    const QString& cover() const;
    float meanScore() const;
    const QString& description() const;
    const QList<Genre>& genres() const;
    const QString& languageName() const;
    qint16 year() const;
    const QString& authorName() const;
    float price() const;
    const Property& language() const;
    const Property& author() const;

private:
    qint64 id_;
    QString title_;
    QString cover_;
    float meanScore_;
    QString description_;
    QList<Genre> genres_;
    Property language_;
    qint16 year_;
    Property author_;
    float price_;

    // Jsonable interface
public:
    QJsonObject toJson() const override;
};

#endif // BOOK_H
