#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

#include "book.h"
#include "genre.h"
#include <optional>
#include "has_method.h"

class BookRepository
{
public:
    BookRepository();
    static std::optional<Book> fetchBookById(int id);
    static QList<Book> fetchBooks(int limit, int page);
    static int getBooksCount();
    static QList<Genre> fetchGenresForBook(int bookId);
    static QList<Book> fetchBooksByTitle(QString searchQuery);
    static void addBook(const Book& book);
    static void deleteBook(int id);

    static void uploadBookFile();

private:
    template <typename... T>
    requires (HasIsNull<T> && ...)
    static bool checkNullVariables(T&&... variables) {
        return (... || variables.isNull());
    }
};

#endif // BOOKREPOSITORY_H
