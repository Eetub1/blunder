import { useState } from "react"
import { Routes, Route, Navigate } from "react-router-dom"

import Login from "./pages/Login"
import SignUp from "./pages/SignUp"
import Frontpage from "./pages/Frontpage"

import DrawBoard from "./components/DrawBoard"
import Message from "./components/Message"
import Header from "./components/Header"

import parseFen from "./utils/parseFen"
import indicesToAlgebraic from "./utils/indicesToAlgebraic"
import algebraicToIndices from "./utils/algebraicToIndices"

import { GameState } from "./types"

// Starting position in FEN notation
const START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

interface MessageData {
    message: string
    isError: boolean
}



function App() {
    const [user, setUser] = useState(() => {
        const loggedUserJSON = window.localStorage.getItem("loggedUser")
        return loggedUserJSON ? JSON.parse(loggedUserJSON) : null
    })

    const [message, setMessage] = useState<MessageData | null>(null)

    const [fen, setFen] = useState(START)
    const [gameState, setGameState] = useState(GameState.ONGOING)
    const [fromSquare, setFromSquare] = useState("")
    const [toSquare, setToSquare] = useState("")


    const handleMove = (fromRow: number, fromCol: number, toRow: number, toCol: number, promotion: string) => {
        const from_square = indicesToAlgebraic([fromRow, fromCol])
        const to_square = indicesToAlgebraic([toRow, toCol])
        let content

        if (promotion) {
            content = {fen, from_square, to_square, promotion: promotion}
        } else {
            content = {fen, from_square, to_square}
        }

        fetch("http://localhost:8000/api/chess/move", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(content)       
        })
            .then(res => res.json())
            .then(data => {
                if (data.fen) {
                    setFen(data.fen)

                    if (data.game_state !== GameState.ONGOING) {
                        setGameState(data.game_state)
                    }

                    setFromSquare(data.from_square)
                    setToSquare(data.to_square)

                } else {
                    console.error("Invalid response from backend: ", data)
                }
            })
            .catch(err => console.error("Failed to send move to backend: ", err))
    }


    const getValidSquares = (from: [number, number]): Promise<number[][]> => {
        const content = { fen, square: indicesToAlgebraic(from) }

        return fetch("http://localhost:8000/api/chess/moves", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(content)
        })
            .then(res => res.json())
            .then(data => data.valid_squares.map(algebraicToIndices))
            .catch(err => {
                console.error("Failed to get valid squares: ", err)
                return []
            })
    }

    const turn = fen.split(" ")[1] === "w" ? "White" : "Black"


    return (
        <>
            <Header user={user} setUser={setUser}/>
            {message && <Message message={message.message} isError={message.isError}/>}
            <Routes>
                <Route path="/" element={<Frontpage/>}/>
                
                <Route path="/board" element={user ?
                    <>
                        <DrawBoard 
                            board={parseFen(fen)} 
                            handleMove={handleMove} 
                            getValidSquares={getValidSquares}
                            gameState={gameState}
                            fromSquare={fromSquare}
                            toSquare={toSquare}/>
                        <div>
                            <h2>Debug section:</h2>
                            <div>Fen string {fen}</div>
                            <div>Turn: {turn}</div>
                            <div>Gamestate: {gameState}</div>
                        </div>
                    </> : <Navigate to="/login"/>
                }/>

                <Route path="login" element={!user ? <Login setUser={setUser} setMessage={setMessage}/> : <Navigate to="/board"/>}/>
                <Route path="signup" element={!user ? <SignUp setUser={setUser} setMessage={setMessage}/> : <Navigate to="/board"/>}/>

            </Routes>


            
        </>
    )
}

export default App
