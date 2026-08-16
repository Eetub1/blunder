import { useState } from "react"
import { Form, Button, Container, Card } from "react-bootstrap"
import { Link } from "react-router-dom"

import { login } from "../services/auth.ts"

import type { User, MessageData } from "../types"

interface LoginProps {
    setUser: (user: User | null) => void
    setMessage: (message: MessageData | null) => void
    setToken: (token: string | null) => void
}

const Login = ({ setUser, setMessage, setToken }: LoginProps) => {
    const [username, setUsername] = useState("")
    const [password, setPassword] = useState("")

    const handleSubmit = async (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault()

        try {
            const data = await login({ username, password })
            console.log("Login successful:", data)
            window.localStorage.setItem("loggedUser", JSON.stringify(data))
            setToken(data.access_token)
            setUsername("")
            setPassword("")
            setUser(data.user)
            setMessage({ message: "Login successful!", isError: false })
            setTimeout(() => {
                setMessage(null)
            }, 4000)
        }  catch (error) {
            const detail = error instanceof Error ? error.message : "Something went wrong"
            setMessage({ message: `Login failed: ${detail}`, isError: true })
            setTimeout(() => setMessage(null), 4000)
        }
    }

    return (
        <Container className="d-flex align-items-center justify-content-center" style={{ minHeight: "80vh" }}>
            <Card style={{ width: "400px"}} className="p-4 shadow">
                <h2 className="text-center mb-4">Login</h2>
                <Form onSubmit={handleSubmit}>
                    <Form.Group className="mb-3">
                        <Form.Label>Username</Form.Label>
                        <Form.Control type="text" value={username} onChange={(e) => setUsername(e.target.value)} />
                    </Form.Group>

                    <Form.Group className="mb-3">
                        <Form.Label>Password</Form.Label>
                        <Form.Control type="password" value={password} onChange={(e) => setPassword(e.target.value)} />
                    </Form.Group>

                    <Button variant="primary" type="submit" className="w-100">Login</Button>

                    <div className="text-center mt-3">
                        <span>Don't have an account? </span>
                        <Link to="/signup" style={{textDecoration: "none"}}>Sign Up</Link>
                    </div>
                </Form>
            </Card>
        </Container>
    )
}

export default Login