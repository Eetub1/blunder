import { useState, useEffect } from "react"
import DrawBoard from "./components/DrawBoard"
import parseFen from "./utils/parseFen"

// Starting position in FEN notation
const START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

function App() {
    const [board, setBoard] = useState<string[][]>(() => parseFen(START))

    // Testing the backend
    useEffect(() => {
        fetch("http://localhost:8000/api/health")
            .then(res => res.json())
            .then(data => console.log("backend:", data))
            .catch(err => console.error("backend unreachable:", err))
    }, [])


    // Just a dummy move handler for testing that everything works.
    // Real app would send the move to the backend
    const handleDummyMove = (fromRow: number, fromCol: number, toRow: number, toCol: number) => {
        // Create a new board to trigger a re-render
        const newBoard = board.map(row => [...row])

        const movingPiece = newBoard[fromRow][fromCol]

        if (movingPiece) {
            newBoard[toRow][toCol] = movingPiece
            newBoard[fromRow][fromCol] = ""
            setBoard(newBoard)
        }
    }

    return (
        <>
            <DrawBoard board={board} onMove={handleDummyMove}/>
        </>
    )
}

export default App
