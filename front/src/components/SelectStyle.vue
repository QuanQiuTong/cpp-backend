<template>
    <el-dropdown class="menu-bar" @command="handleCommand" :hide-on-click="false">
        <span :style="{ marginTop: '9px', marginLeft: '8px' }">
            风格<el-icon class="el-icon--right"><arrow-down /></el-icon>
        </span>
        <template #dropdown>
            <el-dropdown-menu>
                <el-dropdown-item command="0">{{ styles[0] }}</el-dropdown-item>
                <el-dropdown-item command="1">{{ styles[1] }}</el-dropdown-item>
                <el-dropdown-item command="2">{{ styles[2] }}</el-dropdown-item>
                <el-dropdown-item command="3">{{ styles[3] }}</el-dropdown-item>
                <el-dropdown-item command="4">{{ styles[4] }}</el-dropdown-item>
            </el-dropdown-menu>
        </template>
    </el-dropdown>
</template>

<script lang="ts" setup>
import { ArrowDown } from '@element-plus/icons-vue'
import axios from 'axios';


const styles = ['普通', '质朴', '幽默', '严肃', '']

const postStyle = (style: string, url: string = 'http://localhost:8080/tempPrefer/setStyle') => {
    axios.post(url, {
        sessionId: parseInt(localStorage.sessionID),
        style: style
    }, {
        headers: { Authorization: localStorage.token }
    }).then(res => {
        console.log(res.data)
    })
}

const handleCommand = (command: string | number) => {
    postStyle(styles[command])

}
</script>
