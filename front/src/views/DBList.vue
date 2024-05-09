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
import SoftButton from "../components/SoftButton.vue"
import { Delete } from "@element-plus/icons-vue";


const tableData = ref([{
    id: 1,
    problem_id: 1,
    user_id: 1,
    judgement: "solution 1 is better",
    reason: "solution 1 is more efficient",
    created_at: "2024-05-08 14:05:36",
},]);

import axios from "axios";

const orderby = ref('created_at');
const order = ref('desc');
const problem_id = ref(0);
const user_id = ref(0);

const currentPage = ref(1);

const props = () => {
  return {
    problem_id: problem_id.value,
    user_id: user_id.value,
    order_by: orderby.value,
    order: order.value
  }
}

const count = ref(40);

const getTableData = () => {
  console.log(props());

  axios.post('api/count-annotations', props())
    .then(res => {
      console.log(res.data);
      count.value = res.data.count;
    }).catch(err => { console.log(err); });

  axios.post('api/annotations/'+ currentPage.value, props())
  .then(res => {
      console.log(res.data);
      tableData.value = res.data.map(item => {
        item.created_at = item.created_at.split('.')[0]; // 去除秒数后面的部分
        return item;
      });
    }).catch(err => { console.log(err); });
}
getTableData();


const handleCurrentChange = (val) => {
  currentPage.value = val;
    getTableData();
};
const handleSortChange = (val) =>{
  console.log(val);
  orderby.value = val.prop;
  order.value = (val.order === 'ascending' ? 'asc' : 'desc');
  getTableData();
}

const inputPid = ref("")
const inputUid = ref("")
const query = () => {
  console.log('query '+ inputPid.value);
  problem_id.value = inputPid.value ? parseInt(inputPid.value) : 0;
  user_id.value = inputUid.value ? parseInt(inputUid.value) : 0;
  getTableData();
}

const remove = (row) => {
    console.log(row.id)
    axios.post('api/del-annotation', {id: row.id});
    getTableData();
    const index = tableData.value.indexOf(row)
    tableData.value.splice(index, 1)
}
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
    <div class="col-1"></div>
    <div class="menu-bar col-4">
      <!-- <span>Problem: </span> -->
      <input v-model="inputPid" type="text" class="form-control" placeholder="Problem ID" />
    </div>
    <div class="menu-bar col-4">
      <!-- <span>User: </span> -->
      <input v-model="inputUid" type="text" class="form-control" placeholder="User ID" />
    </div>
    <div class="col-2">
     <SoftButton @click="query">Query</SoftButton>
    </div>
  </div>

  <el-table :data="tableData" stripe class="margin-lr" @sort-change="handleSortChange">
    <el-table-column prop="created_at" label="Created At" width="180%" sortable></el-table-column>
    <el-table-column prop="problem_id" label="Problem" width="110%" sortable></el-table-column>
    <el-table-column prop="user_id" label="User ID" width="110%" sortable></el-table-column>
    <el-table-column prop="judgement" label="Judgement" width="240%"></el-table-column>
    <el-table-column prop="reason" label="Reason"></el-table-column>
    <el-table-column label="" width="67%">
            <template #default="{ row }">
                <el-button class="gradient-button-red" type="danger" :icon="Delete" size="small" @click="remove(row)"
                    circle />
            </template>
        </el-table-column>
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

.menu-bar{
  /* display: flex; */
  margin-bottom: 0.5rem;
}

.margin-lr {
  margin-left: 1rem;
  margin-right: 1rem;
}
</style>

<style lang="scss" scoped>
$button-gradient-colors: (
    "red": (#ff0000, #ff7f7f),
    "blue": (#0000ff, #7f7fff),
    // Add more colors here
);

@each $prop, $value in $button-gradient-colors {
    .gradient-button-#{$prop} {
        background: linear-gradient(310deg, nth($value, 1) 0%, nth($value, -1) 100%);
        border: none;
    }
}
</style>