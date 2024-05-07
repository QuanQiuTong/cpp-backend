import axios from 'axios';

const req = (url: string) =>
    axios.post('api/tempPrefer' + url,
        { sessionId: localStorage.sessionID },
        {
            withCredentials: true,
            headers: { 'Authorization': localStorage.token }
        })

export const initPreferences = (): void => {
    console.log('initPreferences');
    if (!localStorage.style) {
        req('/getStyle')
            .then(res => {
                console.log(res.data);
                localStorage.style = res.data.data ? res.data.data : 'default';
                console.log('style not found');
            });
    }
    if (!localStorage.sourceLanguage || !localStorage.targetLanguage) {
        req('/getLanguage')
            .then(res => {
                console.log(res.data);
                localStorage.sourceLanguage = res.data.data ? res.data.data.source_language : 'en';
                localStorage.targetLanguage = res.data.data ? res.data.data.target_language : 'zh';
                console.log('language not found');
            });
    }
}
