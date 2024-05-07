<script setup>
import MyNavBar from "@/components/MyNavBar.vue";
import SoftButton from "@/components/SoftButton.vue";
import SourceLanguage from "@/components/SourceLanguage.vue";
import SelectStyle from "@/components/SelectStyle.vue";
import TargetLanguage from "@/components/TargetLanguage.vue";
import ReplaceList from "@/components/ReplaceList.vue";
import HistoryList from "../components/HistoryList.vue";

import { onMounted, onBeforeUnmount } from 'vue'
import { useStore } from 'vuex'

const store = useStore()
const body = document.getElementsByTagName("body")[0];

onMounted(() => { // 取消侧边栏和上下
  store.commit('toggleEveryDisplay')
  store.commit('toggleHideConfig')
  body.classList.remove("bg-gray-100");
})

onBeforeUnmount(() => {
  store.commit('toggleEveryDisplay')
  store.commit('toggleHideConfig')
  body.classList.add("bg-gray-100");
})


import { ref } from "vue";
import { initPreferences } from "../request/pref";
import { initializeSession } from "../request/session";
import { translate } from "../request/translate.ts";
import { ElMessage } from "element-plus";

initPreferences();
initializeSession();

const input = ref(""), out = ref("");
const submit = () => {
  console.log('submit:\n' + input.value);
  translate(input.value)
    .then((res) => {
      console.log(res.data);
      out.value = res.data.data;
    })
    .catch((err) => {
      console.error(err);
      ElMessage.error(res.data.message);
    });
};

const showReplace = ref(false), showHistory = ref(false);
const viewReplace = () => {
  showReplace.value = true;
}, viewHistory = () => {
  showHistory.value = true;
}

</script>

<template>
  <div class="container top-0 position-sticky z-index-sticky">
    <div class="row">
        <MyNavBar is-blur="blur blur-rounded my-3 py-2 start-0 end-0 mx-4 shadow" btn-background="bg-gradient-success"
          :dark-mode="true" />
    </div>
  </div>
  <div class="mb-7"/>
  <el-drawer v-model="showReplace" title="替换词" size="36%" direction="ltr">
    <ReplaceList />
  </el-drawer>
  <el-drawer v-model="showHistory" title="历史记录" size="36%" direction="rtl" :with-header="false">
    <HistoryList/>
  </el-drawer>

  <div class="container-fluid py-1">

    <div class="row">
      <div class="col-12 col-lg-6">
        <SourceLanguage />
      </div>
      <div class="col-2 col-lg-1">
        <SelectStyle />
      </div>
      <div class="col-10 col-lg-5">
        <TargetLanguage />
      </div>
    </div>

    <div class="row">
      <div class="col-12 col-lg-6">


        <textarea v-model="input" class="form-control" placeholder="输入待翻译的文本"
          :autosize="{ minRows: 40, maxRows: 50 }" />
        <div class="mb-2" />
        <SoftButton variant="gradient" color="success" @click="submit">
          提交
        </SoftButton>

      </div>

      <div class="col-12 col-lg-6">


        <textarea v-model="out" class="form-control" placeholder="翻译结果" autosize />
        <div class="mb-2" />
        <SoftButton variant="gradient" @click="viewReplace">替换词</SoftButton>
        <span class="mx-2" />
        <SoftButton variant="gradient" @click="viewHistory">历史记录</SoftButton>
      </div>
    </div>
  </div>
</template>

<style scoped>
.menu-bar {
  height: 36px;
  background-color: transparent;
  margin-left: 5px;
  margin-right: 5px;
}
</style>