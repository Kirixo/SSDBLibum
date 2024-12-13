<template>
  <div>
    <Header />
    <main class="content">
      <div class="books-container">
        <h1>Books List</h1>
        <!-- Add Book and Delete Book Buttons -->
        <div class="actions">
          <button class="btn primary" @click="openAddBookModal">Add Book</button>
          <button class="btn danger" @click="deleteBooks" :disabled="selectedBookIds.length === 0">
            Delete Selected Books
          </button>
        </div>

        <!-- Book Table -->
        <table class="modern-table">
          <thead>
          <tr>
            <th style="width: 15%;">Title</th>
            <th style="width: 10%;">Cover</th>
            <th style="width: 15%;">Author</th>
            <th style="width: 20%;">Description</th>
            <th style="width: 10%;">Year</th>
            <th style="width: 15%;">Genres</th>
            <th style="width: 10%;">Language</th>
            <th style="width: 10%;">Score</th>
            <th style="width: 10%;">Price</th>
            <th style="width: 20%;">Actions</th>
          </tr>
          </thead>
          <tbody>
          <tr
              v-for="book in books"
              :key="book.id"
              :class="{ expanded: expandedBookId === book.id, selected: selectedBookIds.includes(book.id) }"
              @click="toggleExpandRow(book.id)"
          >
            <td>{{ book.title }}</td>
            <td>
              <img :src="book.cover" :alt="book.title" class="book-cover" />
            </td>
            <td>{{ book.author }}</td>
            <td>
                <span v-if="expandedBookId !== book.id">
                  {{ truncatedText(book.description, 50) }}
                  <span v-if="book.description.length > 50">...</span>
                </span>
              <span v-else>{{ book.description }}</span>
            </td>
            <td>{{ book.year }}</td>
            <td>
                <span v-if="expandedBookId !== book.id">
                  {{ truncatedGenres(book.genres) }}
                  <span v-if="book.genres.length > 2">...</span>
                </span>
              <ul v-else>
                <li v-for="genre in book.genres" :key="genre.id">{{ genre.name }}</li>
              </ul>
            </td>
            <td>{{ book.language }}</td>
            <td>{{ book.mean_score || "N/A" }}</td>
            <td>${{ book.price.toFixed(2) }}</td>
            <td>
              <button class="btn small warning" @click.stop="toggleSelectBook(book.id)">
                {{ selectedBookIds.includes(book.id) ? "Unselect" : "Select" }}
              </button>
              <button class="btn small success" @click.stop="addToCart(book)">
                Add to Cart
              </button>
            </td>
          </tr>
          </tbody>
        </table>

        <!-- Pagination -->
        <div class="pagination">
          <button class="btn small" :disabled="currentPage === 1" @click="fetchBooks(currentPage - 1)">
            &lt; Prev
          </button>
          <span v-for="page in totalPages" :key="page">
            <button
                :class="{ active: currentPage === page }"
                @click="fetchBooks(page)"
                class="btn small pagination-btn"
            >
              {{ page }}
            </button>
          </span>
          <button class="btn small" :disabled="currentPage === totalPages" @click="fetchBooks(currentPage + 1)">
            Next &gt;
          </button>
        </div>
      </div>

      <!-- Add Book Modal -->
      <AddBookModal v-if="showAddBookModal" @close="closeAddBookModal" @add-book="handleAddBook" />
    </main>
    <Footer />
  </div>
</template>

<script>
import axios from "axios";
import Header from "../components/HeaderComponent.vue";
import Footer from "../components/FooterComponent.vue";
import AddBookModal from "../components/AddBookModalComponent.vue";


export default {

  components: { Header, Footer, AddBookModal },
  data() {
    return {
      books: [],
      currentPage: 1,
      totalPages: 1,
      limit: 3,
      expandedBookId: null,
      selectedBookIds: [],
      showAddBookModal: false,
    };
  },
  methods: {
    async fetchBooks(page = 1) {
      try {
        const response = await axios.get(`/api/books/list`, {
          params: { limit: this.limit, page },
        });

        this.books = response.data.books;
        this.totalPages = Math.ceil(response.data.total_count / this.limit);
        this.currentPage = page;
      } catch (error) {
        console.error("Error fetching books:", error);
      }
    },
    toggleExpandRow(bookId) {
      this.expandedBookId = this.expandedBookId === bookId ? null : bookId;
    },
    toggleSelectBook(bookId) {
      if (this.selectedBookIds.includes(bookId)) {
        this.selectedBookIds = this.selectedBookIds.filter((id) => id !== bookId);
      } else {
        this.selectedBookIds.push(bookId);
      }
    },
    truncatedText(text, length) {
      return text.length > length ? text.slice(0, length) : text;
    },
    truncatedGenres(genres) {
      return genres.slice(0, 2).map((genre) => genre.name).join(", ");
    },
    openAddBookModal() {
      this.showAddBookModal = true;
    },
    closeAddBookModal() {
      this.showAddBookModal = false;
    },
    async handleAddBook(newBook) {
      try {
        const response = await axios.post(`/api/books/add`, newBook, null);
        this.books.push(response.data);
        this.closeAddBookModal();
      } catch (error) {
        console.error("Error adding book:", error);
      }
    },
    async deleteBooks() {
      try {
        await Promise.all(
            this.selectedBookIds.map((id) =>
                axios.post(`/api/books/delete`, null, { params: { id } })
            )
        );
        this.books = this.books.filter((book) => !this.selectedBookIds.includes(book.id));
        this.selectedBookIds = [];
      } catch (error) {
        console.error("Error deleting books:", error);
      }
    },
    async addToCart(book) {
      try {
        const userId = this.getUserId(); // Replace with actual user identification logic
        await axios.post(`/api/cart/add`, null, {
          params: { book_id: book.id, user_id: userId },
        });
        alert(`${book.title} has been added to the cart.`);
      } catch (error) {
        console.error("Error adding book to cart:", error);
      }
    },
  },
  created() {
    this.fetchBooks();
  },
};
</script>

<style scoped>
.content {
  padding: 20px;
  max-width: 1200px;
  margin: 0 auto;
}

.books-container {
  text-align: center;
}

.actions {
  margin-bottom: 20px;
}

.actions .btn {
  margin: 0 10px;
}

.modern-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
}

.modern-table th,
.modern-table td {
  padding: 10px;
  text-align: center;
  border: 1px solid #ddd;
}

.modern-table th {
  background-color: #f8f9fa;
}

.modern-table tbody tr:hover {
  background-color: #e8f0fe;
}

.modern-table tr.expanded {
  background-color: #fffbe7;
}

.modern-table tr.selected {
  border: 3px solid #007bff;
}

.book-cover {
  width: 80px;
  height: auto;
  border-radius: 4px;
  border: 1px solid #ddd;
}

.pagination {
  margin-top: 20px;
}

.pagination-btn.active {
  background-color: #007bff;
  color: white;
}

/* Button Styles */
.btn {
  padding: 10px 15px;
  border-radius: 5px;
  cursor: pointer;
}

.btn.primary {
  background-color: #007bff;
  color: white;
}

.btn.danger {
  background-color: #dc3545;
  color: white;
}

.btn.danger:disabled {
  background-color: #e6e6e6;
  color: #aaaaaa;
  cursor: not-allowed;
}

.btn.success {
  background-color: #28a745;
  color: white;
}

.btn.warning {
  background-color: #ffc107;
  color: white;
}

.btn.small {
  padding: 5px 10px;
  font-size: 14px;
}
</style>
