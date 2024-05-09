<script setup>
import MyNavBar from "../components/MyNavBar.vue";
import SoftButton from "../components/SoftButton.vue";
import Markdown from "../components/Markdown.vue";

import { onMounted,onBeforeUnmount, ref } from "vue";
import { useStore } from "vuex";

const store = useStore()
const body = document.getElementsByTagName("body")[0];

onMounted(() => {
  store.commit('toggleEveryDisplay')
  store.commit('toggleHideConfig')
  body.classList.remove("bg-gray-100");
})

onBeforeUnmount(() => {
  store.commit('toggleEveryDisplay')
  store.commit('toggleHideConfig')
  body.classList.add("bg-gray-100");
})



const count = ref(10);
const pid = ref(0); // 数据库中的id
const category = ref("default");
const problem = ref("loading...");
const solution1 = ref("loading...");
const solution2 = ref("loading...");

import axios from "axios";
const getProblemCount = () =>
  axios.get('api/count-problems')
    .then(res => {
      console.log(res.data);
      count.value = res.data.count;
    }).catch(err => { console.log(err); });

const getProblem = (id) =>
  axios.get('api/problem/' + (id - 1))
    .then(res => {
      console.log(res.data);
      pid.value = res.data.id;
      category.value = res.data.category;
      problem.value = res.data.problem;
      solution1.value = res.data.solution1;
      solution2.value = res.data.solution2;
    }).catch(err => { console.log(err); });

const setProblem = () =>
  axios.post('api/set-problem', {
    id: pid.value,
    category: category.value,
    personality: "default",
    history: "none",
    problem: problem.value,
    solution1: solution1.value,
    solution2: solution2.value
  }).then(res => { console.log(res.data); })
    .catch(err => { console.log(err); });

const annotation = (pid, judge, reason) =>{
    let id = sessionStorage.getItem("user_id");
    if(!id) id = localStorage.getItem("user_id");
    if (!id) return alert("Please login first");
    axios.post('/api/annotation', {
      problem_id: pid,
      user_id: parseInt(id),
      judgement: judge,
      reason: reason
    }).then(res => console.log(res.data))
      .catch(err => console.log(err));
}


const currentPage = ref(1);
const handleCurrentChange = (val) => {
  currentPage.value = val;
  getProblem(val);
};



const dialogVisible = ref(false);
const judge = ref("your judgement here");
const reason = ref("");
const check1 = () => {
  dialogVisible.value = true;
  judge.value = "solution 1 is better";
}
const check2 = () => {
  dialogVisible.value = true;
  judge.value = "solution 2 is better";
}
const handleClose = () => {
  dialogVisible.value = false;
  annotation(pid.value, judge.value, reason.value);
  if(++currentPage.value > count.value)
    currentPage.value = 1;
  getProblem(currentPage.value);
};

getProblemCount();
  getProblem(1);
</script>

<template>
  <div class="container top-0 position-sticky z-index-sticky">
    <div class="row">
      <MyNavBar is-blur="blur blur-rounded my-3 py-2 start-0 end-0 mx-4 shadow" btn-background="bg-gradient-success"
        :dark-mode="true" />
    </div>
  </div>

  <el-dialog
    v-model="dialogVisible"
    title="Judgement"
    width="500"
    :before-close="handleClose"
  >
    <input v-model="judge" class="form-control" placeholder="your judgement here" />
    <div class="mb-2"/>
    <textarea v-model="reason" class="form-control long" placeholder="your reason here" />
    <span class="bottom-bar">
      <SoftButton :style="{marginTop:'2rem'}" slot="footer" type="primary" @click="handleClose">Submit</SoftButton>
    </span>
    <div class="mb-2"/>
  </el-dialog>

  <div class="below-nav"/>

  <div class="row margin-lr">
    <div class="col-12 col-lg-4">
      <el-card>
        <Markdown :source="problem" />
      </el-card>
      <span class="bottom-bar"> Category: {{ category }} </span>
    </div>
    <div class="col-12 col-lg-4">
      <el-card>
        <Markdown :source="solution1" />
      </el-card>
      <span class="bottom-bar">
        <SoftButton class="bottom-bar" @click="check1"> Check 1 </SoftButton>
      </span>
    </div>
    <div class="col-12 col-lg-4">
      <el-card>
        <Markdown :source="solution2" />
      </el-card>
      <span class="bottom-bar">
        <SoftButton class="bottom-bar" @click="check2"> Check 2 </SoftButton>
      </span>
    </div>
  </div>

  <div class="row margin-lr" >
    <el-pagination :current-page="currentPage" @update:current-page="handleCurrentChange" :total="count"
      :default-page-size="1" :style="{marginTop: '0.5rem'}"/>
  </div>
</template>

<style scoped>
.margin-lr {
  margin-left: 1rem;
  margin-right: 1rem;
}

.below-nav{
  margin-bottom: 5.4rem;
}

.el-card {
  margin-top: 0rem;
  margin-bottom: 0.5rem;
  white-space: pre-wrap;
  max-height: 78vh;
  overflow-y: auto;
}

.bottom-bar {
  height: 2rem;
  display: flex;
  justify-content: center;
  align-items: center;
  margin-bottom: 0.5rem;
}

.long {
  width: 100%;
  border: 1px solid #ccc;
  padding: 8px;
  height: auto;
  min-height: 40vh;
  overflow-y: auto;
  max-height: 78vh;
  margin-bottom: 0.5rem;
}

.bottom-bar input.form-control {
  height: 2rem;
  margin-top: 1rem;
}

</style>