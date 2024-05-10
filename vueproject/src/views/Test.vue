<template>
    <SoftButton variant="gradient" color="success" @click="submit">
        提交
    </SoftButton>
</template>

<script lang="ts" setup>
import SoftButton from "../components/SoftButton.vue";
import { ref } from 'vue';
import axios from 'axios';

const submit = () => {
    console.log('submit');
    axios.post(
        'api/trans/submitPassage',
        { sessionId: localStorage.sessionID, content: 'hello' },
        { headers: { Authorization: localStorage.token } }
    ).then(res => {
        console.log(res.data);
    }).catch(err => {
        console.error(err);
    });
};
</script>
