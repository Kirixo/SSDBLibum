import QtQuick 2.15
import QtTest 1.0
import bookrepository.h

TestCase {
    name: "FetchBooksTest"

    BookRepository {
        id: backend
    }

    function test_validQuery() {
        var result = backend.fetchBooksByTitle("example");
        compare(result.length, 2, "Should return 2 books for 'example'");
        compare(result[0], "Example Book 1", "First book title should match");
        compare(result[1], "Example Book 2", "Second book title should match");
    }

    function test_emptyQuery() {
        var result = backend.fetchBooksByTitle("");
        compare(result.length, 0, "Should return no books for an empty query");
    }

    function test_partialQuery() {
        var result = backend.fetchBooksByTitle("some");
        compare(result.length, 1, "Should return 1 book for 'some'");
        compare(result[0], "Some Other Book", "Book title should match 'Some Other Book'");
    }
}
