<script setup>
import MyNavBar from "@/components/MyNavBar.vue";
import SoftButton from "@/components/SoftButton.vue";
import Markdown from "@/components/Markdown.vue";

import { onMounted, ref} from "vue";
import { useStore } from "vuex";

const store = useStore();
const body = document.getElementsByTagName("body")[0];

onMounted(() => {
  // 取消侧边栏和上下
  store.commit("noWrappers");
  body.classList.remove("bg-gray-100");

  getProblemCount();
  getProblem(1);
});

const count = ref(10);
const problem = ref("loading...");
const solution1 = ref("loading...");
const solution2 = ref("loading...");

import axios from "axios";
const getProblemCount = () => {
  axios.get('api/count-problems', { headers: { Authorization: localStorage.token } })
    .then(res => {
      console.log(res.data);
      count.value = res.data.count;
    })
    .catch(err => {
      console.log(err);
      
    });
};
const getProblem = (id) => {
  console.log("getProblem", id);
  axios.get('api/problem/' + (id-1), { headers: { Authorization: localStorage.token } })
    .then(res => {
      console.log(res.data);
      problem.value = res.data.problem;
      solution1.value = res.data.solution1;
      solution2.value = res.data.solution2;
    })
    .catch(err => {
      console.log(err);
      
    });
};
const handleCurrentChange = (val) => {
  getProblem(val);
};

</script>

<template>
  <div class="container top-0 position-sticky z-index-sticky">
    <div class="row">
      <MyNavBar
        is-blur="blur blur-rounded my-3 py-2 start-0 end-0 mx-4 shadow"
        btn-background="bg-gradient-success"
        :dark-mode="true"
      />
    </div>
  </div>

  <div class="row" :style="{marginLeft:'1rem', marginRight:'1rem'}">
    <div class="col-12 col-lg-4">
        <el-card>
            <Markdown :source="problem" />
        </el-card>
    </div>
    <div class="col-12 col-lg-4">
        <el-card>
            <Markdown :source="solution1" />
        </el-card>
        <SoftButton @click="check1"> </SoftButton>
    </div>
    <div class="col-12 col-lg-4">
      <el-card>
          <Markdown :source="solution2" />
      </el-card>
    </div>
  </div>
  <div class="row" :style="{marginLeft:'1rem', marginRight:'1rem'}">
  <el-pagination :current-page="1" @update:current-page="handleCurrentChange" :total="count" :default-page-size="1" />
  </div>
</template>

<style scoped>
.el-card {
  /* margin-bottom: 1rem; */
  white-space: pre-wrap;
  max-height: 80vh;
  overflow-y: auto;
}
</style>