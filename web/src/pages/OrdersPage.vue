<template>
  <div>
    <Header />
    <main class="content">
      <div class="orders-container">
        <h1>Orders</h1>

        <!-- Action Buttons -->
        <div class="actions">
          <button class="btn primary" @click="openAuthorModal">Search by Author</button>
          <button class="btn primary" @click="openEmailModal">Search by Email</button>
        </div>

        <!-- Orders Table -->
        <table class="modern-table">
          <thead>
          <tr>
            <th>Order ID</th>
            <th>Username</th>
            <th>Total Cost</th>
            <th>Date</th>
            <th>Info</th>
            <th>Books</th>
          </tr>
          </thead>
          <tbody>
          <tr
              v-for="order in orders"
              :key="order.id"
              :class="{ highlighted: highlightedOrderId === order.id }"
          >
            <td>{{ order.id }}</td>
            <td>{{ order.username }}</td>
            <td>${{ order.cost.toFixed(2) }}</td>
            <td>{{ order.date }}</td>
            <td>{{ order.info }}</td>
            <td>
              <ul>
                <li v-for="book in order.books" :key="book.id">{{ book.title }}</li>
              </ul>
            </td>
          </tr>
          </tbody>
        </table>
      </div>

      <!-- Author Modal -->
      <div v-if="showAuthorModal" class="modal">
        <h3>Search by Author</h3>
        <input
            type="text"
            v-model="authorName"
            placeholder="Enter author name..."
        />
        <div class="modal-actions">
          <button @click="closeAuthorModal" class="btn small">Cancel</button>
          <button @click="searchByAuthor" class="btn small primary">Search</button>
        </div>
      </div>

      <!-- Email Modal -->
      <div v-if="showEmailModal" class="modal">
        <h3>Search by Email</h3>
        <input
            type="text"
            v-model="userEmail"
            placeholder="Enter user email..."
        />
        <div class="modal-actions">
          <button @click="closeEmailModal" class="btn small">Cancel</button>
          <button @click="searchByEmail" class="btn small primary">Search</button>
        </div>
      </div>
    </main>
    <Footer />
  </div>
</template>

<script>
import axios from "axios";
import Header from "../components/HeaderComponent.vue";
import Footer from "../components/FooterComponent.vue";

export default {
  components: { Header, Footer },
  data() {
    return {
      orders: [],
      highlightedOrderId: null,
      showAuthorModal: false,
      showEmailModal: false,
      authorName: "",
      userEmail: "",
    };
  },
  methods: {
    async fetchOrders() {
      try {
        const apiUrl = "/api";
        const response = await axios.get(`${apiUrl}/orders/all?page=1&limit=100`);
        this.orders = response.data.orders.map((order) => ({
          id: order.user.id, // Assuming 'id' is user id for each order
          username: order.user.login || "Unknown",
          cost: order.cost,
          date: order.date,
          info: order.info || "N/A",
          books: order.books.map((book) => ({
            id: book.id,
            title: book.title || "Unknown Title",
          })),
        }));
      } catch (error) {
        console.error("Error fetching orders:", error);
      }
    },
    openAuthorModal() {
      this.showAuthorModal = true;
    },
    closeAuthorModal() {
      this.showAuthorModal = false;
      this.authorName = "";
    },
    openEmailModal() {
      this.showEmailModal = true;
    },
    closeEmailModal() {
      this.showEmailModal = false;
      this.userEmail = "";
    },
    async searchByAuthor() {
      try {
        const apiUrl = "/api";
        const response = await axios.get(`${apiUrl}/orders/mostbooks`, {
          params: { author: this.authorName },
          paramsSerializer: (params) => {
            return Object.keys(params)
                .map((key) => `${encodeURIComponent(key)}=${encodeURIComponent(params[key])}`)
                .join('&');
          },
        });

        const order = response.data.order;
        alert(`Order with most books: ${order.id}`);
        this.highlightedOrderId = order.id;
      } catch (error) {
        console.error("Error searching by author:", error);
      } finally {
        this.closeAuthorModal();
      }
    },
    async searchByEmail() {
      try {
        const apiUrl = "/api";
        const response = await axios.get(`${apiUrl}/orders/largest`, {
          params: { email: this.userEmail },
        });

        const order = response.data.order;
        alert(`Largest order: ${order.id}`);
        this.highlightedOrderId = order.id;
      } catch (error) {
        console.error("Error searching by email:", error);
      } finally {
        this.closeEmailModal();
      }
    }
  },
  created() {
    this.fetchOrders();
  },
};
</script>

<style scoped>
.content {
  padding: 20px;
  max-width: 1200px;
  margin: 0 auto;
}

.orders-container {
  margin-top: 40px;
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

.modern-table tr.highlighted {
  background-color: #ffeeba;
  font-weight: bold;
}

.modal {
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  background: white;
  padding: 20px;
  box-shadow: 0 5px 15px rgba(0, 0, 0, 0.3);
  z-index: 1000;
}

.modal-actions {
  margin-top: 10px;
}

.modal-actions .btn {
  margin-right: 10px;
}

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

.btn.small {
  padding: 5px 10px;
  font-size: 14px;
}
</style>
