import axios from 'axios'

const request = axios.create({
    baseURL: 'api/user',
    timeout: 2000,
    withCredentials: true,
    headers: {
        'Content-Type': 'application/x-www-form-urlencoded'
    }
})
interface User {
    username: string
    password: string
}

interface Response {
    config: object,
    data: { code: number, message: string, data: any },
    headers: object,
    request: object,
    status: number,
    statusText: string
}

export const register = (data: User): Promise<Response> =>
    request.post('/register', data);

export const login = (username: string, password: string): Promise<Response> =>
    request.post('/login', { username, password });

// export function logout() {
//     request.get('/logout').then(res => {
//         return res.data
//     }).catch(err => {
//         console.log(err)
//     })
// }

export const getUserInfo = (): Promise<Response> =>
    request.get('/userInfo');

export const updateUserInfo = (data: User) =>
    request.post('/update', data)

export const updateAvatar = (data: FormData) =>
    request.patch('/updateAvatar', data)