import { useState } from "react"
import { Form, Button, Container, Card } from "react-bootstrap"
import { Link } from "react-router-dom"

import { signUp } from "../services/auth.ts"

export const SignUp = ({ setUser, setMessage }) => {
    const [username, setUsername] = useState("")
    const [password, setPassword] = useState("")

    const handleSignUp = async (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault()

        try {
            const user = await signUp({ username, password })
            window.localStorage.setItem("loggedUser", JSON.stringify(user))
            setUsername("")
            setPassword("")
            setUser(user)
            setMessage({ message: "Sign up successful!", isError: false })
            setTimeout(() => {
                setMessage({ message: "", isError: false })
            }, 4000)
        } catch (error: any) {
            setMessage({ message: `Sign up failed: ${error.message}`, isError: true })
        }
    }


    return (
        <Container className="d-flex align-items-center justify-content-center" style={{ minHeight: "80vh" }}>
            <Card style={{ width: "400px"}} className="p-4 shadow">
                <h2 className="text-center mb-4">Sign up</h2>
                <Form onSubmit={handleSignUp}>
                    <Form.Group className="mb-3">
                        <Form.Label>Username</Form.Label>
                        <Form.Control type="text" value={username} onChange={(e) => setUsername(e.target.value)} />
                    </Form.Group>

                    <Form.Group className="mb-3">
                        <Form.Label>Password</Form.Label>
                        <Form.Control type="password" value={password} onChange={(e) => setPassword(e.target.value)} />
                    </Form.Group>

                    <Button variant="primary" type="submit" className="w-100">Sign up</Button>
                
                    <div className="text-center mt-3">
                        <span>Want to login? </span>
                        <Link to="/login" style={{textDecoration: "none"}}>Login</Link>
                    </div>
                </Form>
            </Card>
        </Container>
    )
}