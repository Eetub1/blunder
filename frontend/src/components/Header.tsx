import { Navbar, Container, Nav, Button } from "react-bootstrap"
import { Link, NavLink } from "react-router-dom"

interface HeaderProps {
    user: { username: string } | null
    setUser: (user: null) => void
}

const Header = ({ user, setUser }: HeaderProps) => {
    const logout = () => {
        setUser(null)
        localStorage.removeItem("loggedUser")
    }

    return (
        <Navbar expand="md" className="shadow-sm" style={{ backgroundColor: "#312e2b" }} variant="dark">
            <Container>
                <Navbar.Brand as={Link} to="/" className="d-flex align-items-center gap-2 fw-bold">
                    <span style={{ fontSize: "1.6rem", lineHeight: 1 }}>♞</span>
                    <span style={{ color: "#eeeed2" }}>Blunder</span>
                </Navbar.Brand>

                <Navbar.Toggle aria-controls="main-nav" />

                <Navbar.Collapse id="main-nav">
                    <Nav className="me-auto">
                        {user && (
                            <Nav.Link as={NavLink} to="/board">Play</Nav.Link>
                        )}
                    </Nav>

                    <Nav className="align-items-center gap-2">
                        {user ? (
                            <>
                                <span className="navbar-text small" style={{ color: "#b9b6b2" }}>
                                    {user.username}
                                </span>
                                <Button onClick={logout} variant="outline-light" size="sm" className="px-3">
                                    Log out
                                </Button>
                            </>
                        ) : (
                            <>
                                <Button as={Link} to="/login" variant="outline-light" size="sm" className="px-3">
                                    Log in
                                </Button>
                                <Button as={Link} to="/signup" size="sm" className="px-3"
                                    style={{ backgroundColor: "#769656", border: "none" }}>
                                    Sign up
                                </Button>
                            </>
                        )}
                    </Nav>
                </Navbar.Collapse>
            </Container>
        </Navbar>
    )
}

export default Header