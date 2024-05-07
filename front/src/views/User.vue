<template>
      <main class="mt-0 main-content main-content-bg">
    <section>
      <div class="page-header min-vh-75">
        <div class="container">
          <div class="row">
            <div class="mx-auto col-xl-4 col-lg-5 col-md-6 d-flex flex-column">
              <div class="mt-8 card card-plain">
                <div class="pb-0 card-header text-start">
                  <h3 class="font-weight-bolder text-success text-gradient">
                    Welcome back
                  </h3>
                  <p class="mb-0">Enter your email and password to sign in</p>
                </div>

                <div class="card-body">
                  <form role="form" class="text-start">
                    <label>Username</label>
                    <soft-input id="username" type="text" placeholder="Username" name="username" />
                    <label>Password</label>
                    <soft-input id="password" type="password" placeholder="Password" name="password" />
                    <soft-switch id="rememberMe" name="rememberMe" checked>
                      Remember me
                    </soft-switch>
                    <div class="text-center">
                      <soft-button class="my-4 mb-2" variant="gradient" color="success" full-width>Sign in
                      </soft-button>
                    </div>
                  </form>
                </div>

                <div class="px-1 pt-0 text-center card-footer px-lg-2">
                  <p class="mx-auto mb-4 text-sm">
                    Don't have an account?
                    <router-link :to="{ name: 'Sign Up' }" class="text-success text-gradient font-weight-bold">Sign
                      up</router-link>
                  </p>
                </div>
              </div>
            </div>
            <div class="col-md-6">
              <div class="top-0 oblique position-absolute h-100 d-md-block d-none me-n8">
                <div class="bg-cover oblique-image position-absolute fixed-top ms-auto h-100 z-index-0 ms-n6" :style="{
                  backgroundImage:
                    'url(' +
                    require('../assets/img/curved-images/curved9.jpg') +
                    ')',
                }"></div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </section>
  </main>
</template>

<script lang="ts" setup>
import SoftButton from "../components/SoftButton.vue";
import SoftInput from "../components/SoftInput.vue";
import axios from 'axios';

interface User {
    createTime: string | Date;
    email: string;
    id: number;
    nickname: string;
    password: string;
    sourceLanguage: string | null;
    targetLanguage: string | null;
    updateTime: string | Date;
    userPic: any;
    username: string;
};

const f = () => {
    axios.get('http://localhost:8080/user/userInfo', {
        headers: { Authorization: localStorage.token }
    }).then(res => {
        console.log(res.data)
    }).catch(err => {
        console.error(err)
    })
}
</script>