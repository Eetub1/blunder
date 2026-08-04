import axios from "axios"

import type { credentials } from "../types"

const BACKEND_ROUTE = "http://localhost:8000"


export const login = async (credentials: credentials) => {
    const response = await axios.post(`${BACKEND_ROUTE}/api/login`, credentials)
    return response.data
}


export const signUp = async (credentials: credentials) => {
    const response = await axios.post(`${BACKEND_ROUTE}/api/signup`, credentials)
    return response.data
}