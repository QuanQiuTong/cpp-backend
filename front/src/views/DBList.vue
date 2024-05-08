<script setup>
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



import MyNavBar from "../components/MyNavBar.vue";

const tableData = ref([{
    id: 1,
    problem_id: 1,
    user_id: 1,
    judgement: "solution 1 is better",
    reason: "solution 1 is more efficient",
    created_at: "2024-05-08 14:05:36",
},]);

import axios from "axios";
const getTableData = (pageID) =>
  axios.get('api/annotations/'+ pageID)
    .then(res => {
      console.log(res.data);
      tableData.value = res.data.map(item => {
        item.created_at = item.created_at.split('.')[0]; // 去除秒数后面的部分
        return item;
      });
    }).catch(err => { console.log(err); });
getTableData(1);

const count = ref(40);
const getCount = () =>
  axios.get('api/count-annotations')
    .then(res => {
      console.log(res.data);
      count.value = res.data.count;
    }).catch(err => { console.log(err); });
getCount();


const currentPage = ref(1);
const handleCurrentChange = (val) => {
  currentPage.value = val;
    getTableData(val);
};
</script>

<template>
  <div class="container top-0 position-sticky z-index-sticky">
    <div class="row">
      <MyNavBar is-blur="blur blur-rounded my-3 py-2 start-0 end-0 mx-4 shadow" btn-background="bg-gradient-success"
        :dark-mode="true" />
    </div>
  </div>

  <div class="below-nav" />

  <el-table :data="tableData" stripe class="margin-lr">
    <el-table-column prop="created_at" label="Created At" width="180%"></el-table-column>
    <el-table-column prop="problem_id" label="Problem" width="90%"></el-table-column>
    <el-table-column prop="user_id" label="User ID" width="90%"></el-table-column>
    <el-table-column prop="judgement" label="Judgement" width="240%"></el-table-column>
    <el-table-column prop="reason" label="Reason"></el-table-column>
  </el-table>

  <div class="row margin-lr">
    <el-pagination :current-page="currentPage" @update:current-page="handleCurrentChange" :total="count"
    :pager-count="21" :default-page-size="10" :style="{marginTop: '0.5rem'}"/>
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
</style>