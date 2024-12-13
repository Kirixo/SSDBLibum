<template>
  <div class="modal-overlay">
    <div class="modal">
      <button @click="$emit('close')" class="close-btn">&#10005;</button>
      <h2>Add New Book</h2>
      <form @submit.prevent="submitForm">
        <div class="form-group">
          <label for="title">Title</label>
          <input id="title" v-model="book.title" required />
        </div>
        <div class="form-group">
          <label for="author">Author</label>
          <input id="author" v-model="book.author" required />
        </div>
        <div class="form-group">
          <label for="cover">Cover URL</label>
          <input id="cover" v-model="book.cover" required />
        </div>
        <div class="form-group">
          <label for="description">Description</label>
          <textarea id="description" v-model="book.description" required></textarea>
        </div>
        <div class="form-group">
          <label for="year">Year</label>
          <input id="year" type="number" v-model="book.year" required />
        </div>
        <div class="form-group">
          <label for="price">Price</label>
          <input id="price" type="number" step="0.01" v-model="book.price" required />
        </div>
        <div class="form-group">
          <label for="language">Language</label>
          <input id="language" v-model="book.language" required />
        </div>
        <div class="form-group">
          <label for="genres">Genres (comma-separated)</label>
          <input
              id="genres"
              v-model="genresInput"
              placeholder="e.g., Fiction, Mystery, Thriller"
          />
        </div>
        <div class="form-actions">
          <button type="submit" class="btn submit">Add Book</button>
          <button type="button" class="btn cancel" @click="$emit('close')">Cancel</button>
        </div>
      </form>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      book: {
        title: "",
        author: "",
        cover: "",
        description: "",
        year: null,
        price: null,
        language: "",
      },
      genresInput: "",
    };
  },
  methods: {
    submitForm() {
      const genres = this.genresInput.split(",").map((genre) => genre.trim());
      this.book.genres = genres.map((name, index) => ({ id: index + 1, name }));
      this.$emit("add-book", this.book);
    },
  },
};
</script>

<style scoped>
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgba(0, 0, 0, 0.3);
  display: flex;
  align-items: center;
  justify-content: center;
}

.modal {
  background: white;
  padding: 20px;
  border-radius: 8px;
  width: 400px;
}

h2 {
  margin-top: 0;
}

.close-btn {
  position: absolute;
  top: 10px;
  right: 10px;
  background: transparent;
  border: none;
  font-size: 18px;
  cursor: pointer;
}

.form-group {
  margin-bottom: 15px;
}

label {
  display: block;
  margin-bottom: 5px;
  font-weight: bold;
}

input,
textarea {
  width: 100%;
  padding: 8px;
  border: 1px solid #ddd;
  border-radius: 4px;
}

textarea {
  height: 80px;
}

.form-actions {
  display: flex;
  justify-content: space-between;
}
.btn {
  padding: 10px;
  border: none;
  cursor: pointer;
  border-radius: 4px;
}
.btn.submit {
  background-color: #28a745;
  color: white;
}
.btn.cancel {
  background-color: #dc3545;
  color: white;
}
</style>
