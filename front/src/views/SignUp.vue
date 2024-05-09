<template>
  <MyNavBar btn-background="bg-gradient-primary" />
  <div class="pt-5 m-3 page-header align-items-start min-vh-50 pb-11 border-radius-lg" :style="{
    backgroundImage:
      'url(' + backgroundImage + ')',
  }">
    <span class="mask bg-gradient-dark opacity-6"></span>
    <div class="container">
      <div class="row justify-content-center">
        <div class="mx-auto text-center col-lg-5">
          <h1 class="mt-5 mb-2 text-white">Welcome!</h1>
        </div>
      </div>
    </div>
  </div>
  <div class="container">
    <div class="row mt-lg-n10 mt-md-n11 mt-n10 justify-content-center">
      <div class="mx-auto col-xl-4 col-lg-5 col-md-7">
        <div class="card z-index-0">
          <div class="pt-4 text-center card-header">
            <h5>Register</h5>
          </div>
          
          <div class="card-body">
            <form role="form" @submit="handleSubmit($event)">
              <div class="mb-3">
                <soft-input id="name" type="text" placeholder="Name" aria-label="Name" />
              </div>
              <div class="mb-3">
                <soft-input id="email" type="email" placeholder="Email" aria-label="Email" />
              </div>
              <div class="mb-3">
                <soft-input id="password" type="password" placeholder="Password" aria-label="Password" />
              </div>
              <soft-checkbox id="flexCheckDefault" name="flexCheckDefault" class="font-weight-light" checked>
                I agree the
                <a href="javascript:;" class="text-dark font-weight-bolder">Terms and Conditions</a>
              </soft-checkbox>

              <div class="text-center">
                <soft-button color="dark" full-width variant="gradient" class="my-4 mb-2">Sign up</soft-button>
              </div>
              <p class="text-sm mt-3 mb-0">
                Already have an account?
                <router-link :to="{ name: 'Sign In' }" class="text-dark font-weight-bolder">
                  Sign in
                </router-link>
              </p>
            </form>
          </div>
        </div>
      </div>
    </div>
  </div>
  <!-- <app-footer /> -->
</template>

<script setup>
import backgroundImage from "@/assets/img/curved-images/curved6.jpg";
import AppFooter from "@/examples/PageLayout/Footer.vue";
import SoftInput from "@/components/SoftInput.vue";
import SoftCheckbox from "@/components/SoftCheckbox.vue";
import SoftButton from "@/components/SoftButton.vue";

import { useStore } from 'vuex'

const store = useStore()

const toggleEveryDisplay = () => store.commit('toggleEveryDisplay')
const toggleHideConfig = () => store.commit('toggleHideConfig')

import { onMounted, onBeforeUnmount } from 'vue'

onMounted(() => {
  toggleEveryDisplay()
  toggleHideConfig()
})

onBeforeUnmount(() => {
  toggleEveryDisplay()
  toggleHideConfig()
})

import { useRouter } from 'vue-router'
import axios from "axios";
import MyNavBar from "../components/MyNavBar.vue";

const router = useRouter()
const handleSubmit = (e) => {
  e.preventDefault()
  axios.post('api/signup',{
      username: e.target.name.value,
      email: e.target.email.value,
      password: e.target.password.value
    }).then((res) => {
    console.log(res.data)
    if(res.data.code === 1) {
      alert(res.data.message)
      return
    }
    router.push('/sign-in')
  }).catch((err) => {
    console.log('error')
    //console.error(err)
  })

}
</script>
