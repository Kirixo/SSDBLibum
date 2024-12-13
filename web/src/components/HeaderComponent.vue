<template>
  <header class="header">
    <div class="container">
      <h1 class="logo">My Bookstore</h1>
      <nav>
        <ul class="nav-links">
          <li><a href="/">Home</a></li>
          <li><a href="/">Books</a></li>
          <li><a href="/orders">Orders</a></li>
          <li><a href="/contact">Contact</a></li>
        </ul>
      </nav>
      <div class="auth-buttons">
        <button @click="showLoginModal" class="btn login">Login</button>
        <button @click="showSignupModal" class="btn signup">Signup</button>
      </div>
    </div>

    <!-- Modals -->
    <LoginModal
        v-if="isLoginModalVisible"
        :isVisible="isLoginModalVisible"
        @close="isLoginModalVisible = false"
        @login="handleLogin"
    />
    <SignupModal
        v-if="isSignupModalVisible"
        :isVisible="isSignupModalVisible"
        @close="isSignupModalVisible = false"
        @signup="handleSignup"
    />
  </header>
</template>

<script>
import LoginModal from "./LoginModalComponent.vue";
import SignupModal from "./SignupModalComponent.vue";

export default {
  components: { LoginModal, SignupModal },
  data() {
    return {
      isLoginModalVisible: false,
      isSignupModalVisible: false,
    };
  },
  methods: {
    showLoginModal() {
      this.isLoginModalVisible = true;
    },
    showSignupModal() {
      this.isSignupModalVisible = true;
    },
    handleLogin(credentials) {
      console.log("Logged in with:", credentials);
      this.isLoginModalVisible = false;
    },
    handleSignup(credentials) {
      console.log("Signed up with:", credentials);
      this.isSignupModalVisible = false;
    },
  },
};
</script>

<style scoped>
.header {
  background: #333;
  color: #fff;
  padding: 15px 20px;
}
.container {
  display: flex;
  justify-content: space-between;
  align-items: center;
}
.logo {
  font-size: 1.5em;
  font-weight: bold;
}
.nav-links {
  list-style: none;
  display: flex;
  gap: 15px;
}
.nav-links a {
  color: #fff;
  text-decoration: none;
}
.nav-links a:hover {
  text-decoration: underline;
}
.auth-buttons {
  display: flex;
  gap: 10px;
}
.btn {
  padding: 10px 15px;
  cursor: pointer;
  border: none;
  border-radius: 4px;
  color: white;
}
.btn.login {
  background-color: #007bff;
}
.btn.signup {
  background-color: #28a745;
}
</style>
