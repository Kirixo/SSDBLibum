import {
  createRouter,
  createWebHistory,
} from 'vue-router';
import MainPage from '@/pages/MainPage.vue';
import BookPage from '@/pages/BookPage.vue';
import CartPage from '@/pages/CartPage.vue';
import TestPage from '@/pages/testModalsPage.vue';
import OrdersPage from "@/pages/OrdersPage.vue";

export default createRouter({
  history: createWebHistory(),
  routes: [{
    path: '/',
    name: 'MainPage',
    components: {
      default: MainPage,
    },
  }, {
    path: '/orders',
    name: 'OrdersPage',
    props: true,
    component: OrdersPage,
  }, {
    path: '/book/:id',
    name: 'BookPage',
    props: true,
    component: BookPage,
  }, {
    path: '/cart',
    name: 'CartPage',
    props: true,
    component: CartPage,
  },
  {
    path: '/test',
    name: 'TestPage',
    component: TestPage,
  }],
});
