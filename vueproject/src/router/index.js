import { createRouter, createWebHistory, createWebHashHistory } from "vue-router";
import Dashboard from "@/views/Dashboard.vue";
import Tables from "@/views/Tables.vue";
import Billing from "@/views/Billing.vue";
import VirtualReality from "@/views/VirtualReality.vue";
import Profile from "@/views/Profile.vue";
import Rtl from "@/views/Rtl.vue";
import SignIn from "@/views/SignIn.vue";
import SignUp from "@/views/SignUp.vue";
import { ElMessage } from 'element-plus'

const routes = [
  {
    path: "/",
    name: "/",
    redirect: "/problems",
  },
  {
    path: "/dashboard",
    name: "Dashboard",
    component: Dashboard,
  },
  {
    path: "/tables",
    name: "Tables",
    component: Tables,
  },
  {
    path: "/billing",
    name: "Billing",
    component: Billing,
  },
  {
    path: "/virtual-reality",
    name: "Virtual Reality",
    component: VirtualReality,
  },
  {
    path: "/profile",
    name: "Profile",
    component: Profile,
  },
  {
    path: "/rtl-page",
    name: "Rtl",
    component: Rtl,
  },
  {
    path: "/sign-in",
    name: "Sign In",
    component: SignIn,
  },
  {
    path: "/sign-up",
    name: "Sign Up",
    component: SignUp,
  },
  {
    path: "/user",
    name: "User",
    component: () => import("@/views/User.vue"),
  },
  {
    path: "/test",
    name: "Test",
    component: () => import("@/views/Test.vue"),
  },
  {
    path: "/problems",
    name: "Problems",
    component: () => import("@/views/DBProblems.vue"),
  },
  {
    path: "/manage",
    name: "Manage",
    component: () => import("@/views/DBManage.vue"),
  },
  {
    path: "/list",
    name: "List",
    component: () => import("@/views/DBList.vue"),
  },
  {
    path: '/sign-out',
    name: 'Sign Out',
    beforeEnter: (to, from, next) => {
      sessionStorage.removeItem('token');
      localStorage.removeItem('token');
      next('/sign-in');
      ElMessage.success('Succeccfully Signed Out!')
    }
  },
];

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes,
  linkActiveClass: "active",
});

export default router;
