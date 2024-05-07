<template>
    <el-menu :default-active="activeIndex" class="menu-bar" mode="horizontal" @select="handleSelect">
        <el-menu-item index="0">{{ langList[0] }}</el-menu-item>
        <el-menu-item index="1">{{ langList[1] }}</el-menu-item>
        <el-menu-item index="2">{{ langList[2] }}</el-menu-item>
        <el-sub-menu index="3">
            <template #title>其他</template>
            <el-menu-item index="3-0">德语</el-menu-item>
            <el-menu-item index="3-1">日语</el-menu-item>
            <el-menu-item index="3-2">印地语</el-menu-item>
            <el-sub-menu index="3-3">
                <template #title>item four</template>
                <el-menu-item index="2-4-1">item one</el-menu-item>
                <el-menu-item index="2-4-2">item two</el-menu-item>
                <el-menu-item index="2-4-3">item three</el-menu-item>
            </el-sub-menu>
        </el-sub-menu>
    </el-menu>
</template>

<script lang="ts" setup>
import axios from 'axios';
import { ref } from 'vue'

const langList = ['自动识别', '英语', '中文', { "3-0": "德语" }]
const langMap = {
    0: '自动识别',
    1: '英语',
    2: '中文',
    "3-0": '德语',
    "3-1": "日语",
    "3-2": "印地语",
}

const activeIndex = ref('0') // 好像不会被更新
const handleSelect = (key: string, keyPath: string[]) => {
    console.log(key, keyPath)
    if (typeof langMap[key] === 'string')
        localStorage.sourceLanguage = langMap[key]
    axios.post(
        'http://localhost:8080/tempPrefer/setLanguage', {
        sessionId: parseInt(localStorage.sessionID),
        source_language: localStorage.sourceLanguage,
        target_language: localStorage.targetLanguage
    }, {
        headers: { Authorization: localStorage.token }
    }).then(res => {
        console.log(res.data)
    })
}
</script>