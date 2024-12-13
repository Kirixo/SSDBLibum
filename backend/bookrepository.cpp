#include "bookrepository.h"
#include "logger.h"
#include "dbcontroller.h"
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

BookRepository::BookRepository() {}

std::optional<Book> BookRepository::fetchBookById(int id)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[fetchBookById] Database not open!"), Logger::LogLevel::Warning);
        return std::nullopt;
    }

    QString queryString = R"(
        SELECT b.id, b.title, b.description, a.name AS authors,
            b.year, b.price, l.lang_name, b.cover
        FROM books AS b
        LEFT JOIN languages AS l ON b.language_id = l.id
        LEFT JOIN authors AS a ON b.authors_id = a.id
        WHERE b.id = :id
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        Logger::instance().log(QString("[fetchBookById] Database query error!")
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
        return std::nullopt;
    }

    Book book;
    book.setId(query.value("id").toInt());
    book.setTitle(query.value("title").toString());
    book.setDescription(query.value("description").toString());
    book.setAuthorName(query.value("authors").toString());
    book.setPrice(query.value("price").toFloat());
    book.setCover(query.value("cover").toString());
    book.setLanguageName(query.value("lang_name").toString());

    book.setGenres(fetchGenresForBook(book.id()));

    return book;
}

QList<Book> BookRepository::fetchBooks(int limit, int page)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[fetchBooks] Database not open!"), Logger::LogLevel::Warning);
        return {};
    }

    QString queryString = R"(
        SELECT b.id, b.title, b.description, a.name AS author_name,
            b.year, b.price, l.lang_name, b.cover
        FROM books AS b
        LEFT JOIN languages AS l ON b.language_id = l.id
        LEFT JOIN authors AS a ON b.authors_id = a.id
        ORDER BY b.id DESC
        LIMIT :limit OFFSET :offset;
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":limit", limit);
    query.bindValue(":offset", (page - 1) * limit);

    QList<Book> books;
    if (query.exec()) {
        while (query.next()) {
            Book book;
            book.setId(query.value("id").toInt());
            book.setTitle(query.value("title").toString());
            book.setDescription(query.value("description").toString());
            book.setAuthorName(query.value("author_name").toString());
            book.setPrice(query.value("price").toFloat());
            book.setYear(query.value("year").toInt());
            book.setLanguageName(query.value("lang_name").toString());
            book.setCover(query.value("cover").toString());
            book.setGenres(fetchGenresForBook(book.id()));
            books.append(std::move(book));
        }
    } else {
        Logger::instance().log(QString("[fetchBooks] Database query error!")
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
    }

    return books;
}

int BookRepository::getBooksCount()
{
    QSqlDatabase db = DBController::getDatabase();

    QString queryString = R"(
        SELECT COUNT(1)
        FROM books;
    )";

    QSqlQuery query(db);
    query.prepare(queryString);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

QList<Genre> BookRepository::fetchGenresForBook(int bookId)
{
    QSqlDatabase db = DBController::getDatabase();
    QList<Genre> genres;

    QString queryString = R"(
        SELECT g.id, g.genre_name
        FROM book_genre bg
        LEFT JOIN genres AS g ON g.id = bg.genre_id
        WHERE bg.book_id = :id
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":id", bookId);

    if (query.exec()) {
        while (query.next()) {
            Genre tmpGenre;
            tmpGenre.setId(query.value("id").toInt());
            tmpGenre.setName(query.value("genre_name").toString());
            genres.append(std::move(tmpGenre));
        }
    }

    return genres;
}

QList<Book> BookRepository::fetchBooksByTitle(QString searchQuery)
{
    return {};
}

void BookRepository::addBook(const Book &book)
{
    if(checkNullVariables(book.title())) {
        Logger::instance().log(QString("[%1] Title or Price is NULL!")
                                   .arg(__func__), Logger::LogLevel::Warning);
        throw std::runtime_error("Title and Price must contain values.");
    }

    QSqlDatabase db = DBController::getDatabase();

    QString queryString = QString(R"(
        CALL add_book (
            %1::VARCHAR, %2::TEXT, %3::INTEGER,
            %4::INTEGER, %5::SMALLINT, %6::NUMERIC, %7::VARCHAR
        );
    )") .arg("\'"+book.title()+"\'")
        .arg(book.description().isEmpty() ? "NULL" : "\'"+book.description()+"\'")
        .arg(book.language().id ? QString::number(book.language().id) : "NULL")
        .arg(book.author().id ? QString::number(book.author().id) : "NULL")
        .arg(book.year() ? QString::number(book.year()) : "NULL")
        .arg(book.price() ? QString::number(book.price()) : "NULL")
        .arg(book.cover().isEmpty() ? "NULL" : "\'"+book.cover()+"\'");
    auto q = db.exec(queryString);
    if(!q.isActive()) {
        Logger::instance().log(QString("[%1] Database query error!")
                                   .arg(__func__), Logger::LogLevel::Error);
        throw std::runtime_error(QString("Error has occurred: ").toStdString());
    }
}

void BookRepository::deleteBook(int id)
{
    QSqlDatabase db = DBController::getDatabase();

    QString queryString = R"(
        DELETE FROM books
        WHERE id = :id;
    )";

    QSqlQuery query(db);
    query.prepare(queryString);

    query.bindValue(":id", id);

    if(!query.exec()) {
        Logger::instance().log(QString("[%1] Database query error!")
                                   .arg(__func__)
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
        throw std::runtime_error(QString("Error has occurred: ")
                                     .append(query.lastError().text()).toStdString());
    }
}

void BookRepository::uploadBookFile()
{

}


