<template>
    <div :style="{ display: 'flex', justifyContent: 'space-between' }">
        <span>历史记录</span>
        <el-button class="gradient-button-green" type="danger" @click="create">
            <span>新建会话</span>
            <span class="mx-2" />
            <i class="fas fa-arrow-right"></i>
        </el-button>
    </div>

    <el-table :data="gridData">
        <el-table-column label="ID" prop="id" width="50%" />
        <el-table-column label="源语言" prop="src" />
        <el-table-column label="目标语言" prop="tgt" />
        <el-table-column label="" width="60%">
            <template #default="{ row }">
                <el-button class="gradient-button-blue" type="danger" :icon="Position" @click="select(row)" size="small"
                    circle />
            </template>
        </el-table-column>
    </el-table>
</template>

<script lang="ts" setup>
import { Position } from "@element-plus/icons-vue";
import { ref } from 'vue'
import { getSessionList, sessionOutline, newSession } from "../request/session";

const gridData = ref([
    { id: "0", src: "en", tgt: "zh" },
])

getSessionList().then(res => {
    console.log(res.data)
    gridData.value = res.data.data.map((item: sessionOutline) =>
        ({ "id": item.sessionId, "src": item.sourceLanguage, "tgt": item.targetLanguage })
    );
})

const select = (row) => {
    console.log(row)
    localStorage.sessionID = row.id
    location.href = '/translate'
}

const create = () =>
    newSession().then(res => {
        console.log(res.data)
        localStorage.sessionID = res.data.data.sessionId
        location.href = '/translate'
    })

</script>

<style scoped>
/* make IntelliSense happy */
</style>

<style lang="scss" scoped>
$button-gradient-colors: (
    "red": (#ff0000, #ff7f7f),
    "blue": (#0000ff, #7f7fff),
    "green": (#3bdc3b, #9ceb47),
    // Add more colors here
);

@each $prop, $value in $button-gradient-colors {
    .gradient-button-#{$prop} {
        background: linear-gradient(310deg, nth($value, 1) 0%, nth($value, -1) 100%);
        border: none;
    }
}
</style>