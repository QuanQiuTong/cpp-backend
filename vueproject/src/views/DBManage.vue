<script setup>
import MyNavBar from "../components/MyNavBar.vue";
import SoftButton from "../components/SoftButton.vue";
import { ElMessage } from 'element-plus'

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
  }).then(res => { console.log(res.data); ElMessage.success('Saved!');})
    .catch(err => { console.log(err); });

const delProblem = () =>{
  console.log(pid.value);
  axios.post('api/del-problem',
   {id: pid.value}
  ).then(res => {
    console.log(res.data);
    getProblemCount();
    if(currentPage.value > 1)
      currentPage.value -= 1;
    getProblem(currentPage.value);
    ElMessage.warning('Deleted!');
  })
    .catch(err => { console.log(err); });}

const newProblem = () =>
  axios.post('api/add-problem', {
    category: 'basic math',
    personality: "default",
    history: "none",
    problem: '1+1=\n\t--' + (new Date()).toLocaleString(),
    solution1: '1',
    solution2: '2'
  }).then(async (res) => {
    console.log(res.data);
    await getProblemCount();
    currentPage.value = count.value;
    getProblem(currentPage.value);
    ElMessage('New Problem Created.');
  })
  .catch(err => { console.log(err); });

const currentPage = ref(1);
const handleCurrentChange = (val) => {
  currentPage.value = val;
  getProblem(val);
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

  <div class="below-nav" />


  <div class="row margin-lr">
    <div class="col-12 col-lg-4">
      <textarea v-model="problem" class="form-control long" autosize/>
    </div>
    <div class="col-12 col-lg-4">
      <textarea v-model="solution1" class="form-control long" autosize/>
    </div>
    <div class="col-12 col-lg-4">
      <textarea v-model="solution2" class="form-control long" autosize/>
    </div>
  </div>

  <div class="row margin-lr">
    <div class="col-4">
      <span class="bottom-bar">
        <input class="form-control" v-model="category" placeholder="Category" />
      </span>
    </div>
    <div class="col-3">
        <SoftButton class="bottom-bar" @click="setProblem"> Save </SoftButton>
    </div>
    <div class="col-3">
        <SoftButton class="bottom-bar" color="danger" @click="delProblem"> Delete </SoftButton>
    </div>
    <div class="col-2">
        <SoftButton class="bottom-bar" @click="newProblem"> New </SoftButton>
    </div>
  </div>

  <div class="row margin-lr">
    <el-pagination :current-page="currentPage" @update:current-page="handleCurrentChange" :total="count"
    :pager-count="21" :default-page-size="1" :style="{marginTop: '0.5rem'}"/>
  </div>
</template>

<style scoped>
.below-nav{
  margin-bottom: 5.4rem;
}

.margin-lr {
  margin-left: 1rem;
  margin-right: 1rem;
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
  border-radius: 4px;
  padding: 8px;
  box-sizing: border-box;
  height: auto;
  min-height: 40vh;
  overflow-y: auto;
  max-height: 78vh;
  box-shadow: 0px 2px 6px rgba(0, 0, 0, 0.2); /* 增加阴影范围 */
  margin-bottom: 0.5rem;
}

.bottom-bar input.form-control {
  height: 2rem;
}

</style>