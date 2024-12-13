#include "bookhandler.h"
#include <qjsondocument.h>
#include "genrerepository.h"
#include "logger.h"
#include "responsefactory.h"


BookHandler::BookHandler() {}

QHttpServerResponse BookHandler::getBook(const QHttpServerRequest &request)
{
    bool ok;
    int bookId = request.query().queryItemValue("id").toInt(&ok);

    Logger::instance().log(QString("[getBook request]: id = %1").arg(bookId), Logger::LogLevel::Info);

    if (!ok) {
        return ResponseFactory::createResponse("Invalid book ID.", QHttpServerResponse::StatusCode::BadRequest);
    }

    std::optional<Book> book = BookRepository::fetchBookById(bookId);

    if (book) {
        QByteArray responseData = QJsonDocument(book->toJson()).toJson(QJsonDocument::Compact);
        return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
    }

    return ResponseFactory::createResponse("Book not found.", QHttpServerResponse::StatusCode::NotFound);
}

QHttpServerResponse BookHandler::getBookList(const QHttpServerRequest &request)
{
    const int defautLimit = 24;
    const int defaultPage = 1;

    bool isLimitOk;
    int limit = request.query().queryItemValue("limit").toInt(&isLimitOk);
    if (!isLimitOk) {
        limit = defautLimit;
    }

    bool isPageOk;
    int page = request.query().queryItemValue("page").toInt(&isPageOk);
    if (!isPageOk) {
        page = defaultPage;
    }

    Logger::instance().log(QString("[getBookList request]: limit =  %1, page = %2").arg(limit)
                               .arg(page), Logger::LogLevel::Info);

    QList<Book> books = BookRepository::fetchBooks(limit, page);

    if (!books.isEmpty()) {
        QJsonArray bookArray;
        for (const Book &book : books) {
            bookArray.append(book.toJson());
        }

        QJsonObject responseJson;
        responseJson["total_count"] = BookRepository::getBooksCount();
        responseJson["books"] = bookArray;

        QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);

        return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
    }

    Logger::instance().log(QString("[getBookList request] Not Found: limit =  %1, page = %2").arg(limit)
                               .arg(page), Logger::LogLevel::Warning);

    return ResponseFactory::createResponse("Books not found.", QHttpServerResponse::StatusCode::NotFound);
}

QHttpServerResponse BookHandler::getGenresList(const QHttpServerRequest &request)
{
    QList<Genre> genres = GenreRepository::getAllGenres();

    Logger::instance().log(QString("[getGenreList request]"), Logger::LogLevel::Info);

    if (!genres.isEmpty()) {
        QJsonArray genreArray;
        for (const Genre &genre : genres) {
            genreArray.append(genre.toJson());
        }

        QJsonObject responseJson;
        responseJson["genres"] = genreArray;

        QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);

        return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
    }

    Logger::instance().log(QString("[getGenreList request] Not Found"), Logger::LogLevel::Warning);

    return ResponseFactory::createResponse("Genres not found.", QHttpServerResponse::StatusCode::NotFound);
}

QHttpServerResponse BookHandler::addBook(const QHttpServerRequest &request)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(request.body(), &err).object();

    Book book;
    if (!json.contains("title") || !json.contains("price"))
        return ResponseFactory::createResponse("Invalid title or price.",
                                               QHttpServerResponder::StatusCode::BadRequest);
    book.setTitle(json.value("title").toString());
    book.setAuthor(Book::Property(json.value("author_id").toInt(), nullptr));
    book.setCover(json.value("cover").toString());
    book.setDescription(json.value("description").toString());
    book.setYear(json.value("year").toInt());
    book.setPrice(json.value("price").toDouble());
    book.setLanguage(Book::Property(json.value("language_id").toInt(), nullptr));
    QList<Genre> genres;
    for(auto genre : json.value("genres").toArray()) {
        genres.append(Genre(genre.toInt(), nullptr));
    }
    book.setGenres(genres);

    Logger::instance().log(QString("[addBook request]: title = %1")
                               .arg(book.title()), Logger::LogLevel::Info);
    try {
        BookRepository::addBook(book);
        return ResponseFactory::createResponse("Book has been successfully added.",
                                               QHttpServerResponse::StatusCode::Ok);
    } catch (const std::exception& e) {
        Logger::instance().log(QString("[addBook request]: %1")
                                   .arg(e.what()), Logger::LogLevel::Error);
        return ResponseFactory::createResponse(e.what(),
                                               QHttpServerResponse::StatusCode::InternalServerError);
    }
}

QHttpServerResponse BookHandler::deleteBook(const QHttpServerRequest &request)
{
    Logger::instance().log(QString("[deleteBook request] started"), Logger::LogLevel::Debug);
    bool ok;
    int bookId = request.query().queryItemValue("id").toInt(&ok);
    if (!ok) {
        return ResponseFactory::createResponse("Invalid book ID.", QHttpServerResponse::StatusCode::BadRequest);
    }

    Logger::instance().log(QString("[deleteBook request]: book_id = %1")
                               .arg(bookId), Logger::LogLevel::Info);

    try {
        BookRepository::deleteBook(bookId);
        const QByteArray responseMessage = "Book has been successfully deleted.";
        Logger::instance().log(QString("[deleteBook request]: %1 book_id = %2")
                                   .arg(responseMessage).arg(bookId), Logger::LogLevel::Info);
        return ResponseFactory::createJsonResponse(responseMessage, QHttpServerResponse::StatusCode::Ok);
    } catch  (const std::exception& e) {
        const QByteArray responseMessage = e.what();
        Logger::instance().log(QString("[removeBook request]: %1 book_id = %2")
                                   .arg(responseMessage).arg(bookId), Logger::LogLevel::Error);
        return ResponseFactory::createJsonResponse(responseMessage, QHttpServerResponse::StatusCode::InternalServerError);
    }
}
