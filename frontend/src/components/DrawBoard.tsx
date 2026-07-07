/*Some of the UI styles are AI generated*/

import { useState } from "react"
import { type PendingPromotion, GameState } from "../types"
import algebraicToIndices from "../utils/algebraicToIndices"

import whitePawn from "../assets/pieces/P.svg"
import blackPawn from "../assets/pieces/p.svg"
import whiteKnight from "../assets/pieces/N.svg"
import blackKnight from "../assets/pieces/n.svg"
import whiteBishop from "../assets/pieces/B.svg"
import blackBishop from "../assets/pieces/b.svg"
import whiteRook from "../assets/pieces/R.svg"
import blackRook from "../assets/pieces/r.svg"
import whiteQueen from "../assets/pieces/Q.svg"
import blackQueen from "../assets/pieces/q.svg"
import whiteKing from "../assets/pieces/K.svg"
import blackKing from "../assets/pieces/k.svg"

interface DrawBoardProps {
    board: string[][]
    handleMove: (fromRow: number, fromCol: number, toRow: number, toCol: number, promotion: string) => void
    getValidSquares: (from: [number, number]) => Promise<number[][]>
    gameState: GameState
    fromSquare: string
    toSquare: string
}

const CELL_SIZE = 64

const getPieceImage = (char: string) => {
    if (!char) return undefined // Return undefined for empty cells

    // TODO, get these some otherway, dont hotlink them
    switch (char) {
        case "P": return whitePawn
        case "N": return whiteKnight
        case "B": return whiteBishop
        case "R": return whiteRook
        case "Q": return whiteQueen
        case "K": return whiteKing
        case "p": return blackPawn
        case "n": return blackKnight
        case "b": return blackBishop
        case "r": return blackRook
        case "q": return blackQueen
        case "k": return blackKing
        default: return undefined
    }
}


const DrawBoard = ({ board, handleMove, getValidSquares, gameState, fromSquare, toSquare }: DrawBoardProps) => {
    const [highlightCells, setHighlightCells] = useState<number[][]>([])
    const [pendingPromotion, setPendingPromotion] = useState<PendingPromotion | null>(null)
    const [isBoardFlipped, setIsBoardFlipped] = useState(false)

    const handleDragStart = async (e: React.DragEvent, row: number, col: number) => {
        e.dataTransfer.setData("text/plain", JSON.stringify({ row, col }))
        const validSquares = await getValidSquares([row, col])
        setHighlightCells(validSquares)
    }

    const handleDragOver = (e: React.DragEvent) => {
        e.preventDefault()
    }

    const handleDrop = (e: React.DragEvent, toRow: number, toCol: number) => {
        setHighlightCells([])

        e.preventDefault()
        try {
            const data = e.dataTransfer.getData("text/plain")
            if (!data) return

            const parsedObject = JSON.parse(data)
            const fromRow = parsedObject.row
            const fromCol = parsedObject.col

            const piece = board[fromRow][fromCol]

            let promotionAvailable = false
            let isWhite = false
            if (piece.toLowerCase() === "p") {
                isWhite = piece === piece.toUpperCase()
                if (!isWhite && toRow === 7) promotionAvailable = true
                if (isWhite && toRow === 0) promotionAvailable = true
            }

            if (promotionAvailable) {
                setPendingPromotion({ fromRow, fromCol, toRow, toCol, isWhite })
            } else {
                handleMove(fromRow, fromCol, toRow, toCol, "")
            }
        } catch (err) {
            console.error("Failed to parse drag data", err)
        }
    }

    const choosePromotion = (pieceType: string) => {
        if (!pendingPromotion) return
        const { fromRow, fromCol, toRow, toCol, isWhite } = pendingPromotion

        const newPiece = isWhite ? pieceType.toUpperCase() : pieceType.toLowerCase()
        handleMove(fromRow, fromCol, toRow, toCol, newPiece)
        setPendingPromotion(null)
    }

    const promotionChoices = ["q", "r", "b", "n"]

    const [ validFromRow, validFromCol ] = algebraicToIndices(fromSquare)
    const [ validToRow, validToCol ] = algebraicToIndices(toSquare)

    // these are iterated over. Easy way to flip the board
    const indexes = isBoardFlipped ? [7,6,5,4,3,2,1,0] : [0,1,2,3,4,5,6,7]

    return (
        <>
            <div>
                <button onClick={() => {setIsBoardFlipped(!isBoardFlipped)}}>flip board</button>
                <div>Is board flipped: {isBoardFlipped ? "Yes" : "No"}</div>
            </div>

            <div id="boardContainer">
                {indexes.map((rowIndex) => (
                    <div key={rowIndex} style={{ display: "flex" }}>
                        {indexes.map((colIndex) => {
                            const cell = board[rowIndex][colIndex]
                            const imgSrc = getPieceImage(cell)
                            const isLightSquare = (rowIndex + colIndex) % 2 === 0

                            // calculate if cell needs a col or row indicator
                            const showRank = colIndex === 0
                            const showFile = rowIndex === 7
                            const rankLabel = 8 - rowIndex
                            const fileLabel = "abcdefgh"[colIndex]
                            const labelColor = isLightSquare ? "#769656" : "#eeeed2"

                            // returns an individual cell
                            return (
                                <div
                                    className={`
                                        ${highlightCells.some(([r, c]) => r === rowIndex && c === colIndex) ? "highlight" : ""} 
                                        ${(rowIndex === validFromRow && colIndex === validFromCol) ? "lastMove" : ""}
                                        ${(rowIndex === validToRow && colIndex === validToCol) ? "lastMove" : ""}`}
                                    key={colIndex}
                                    onDragOver={handleDragOver}
                                    onDrop={(e) => handleDrop(e, rowIndex, colIndex)}
                                    style={{
                                        position: "relative",
                                        display: "flex",
                                        justifyContent: "center",
                                        alignItems: "center",
                                        width: CELL_SIZE,
                                        height: CELL_SIZE,
                                        backgroundColor: isLightSquare ? "#eeeed2" : "#769656"
                                    }}
                                >
                                    {showRank && (
                                        <span style={{
                                            position: "absolute", top: 2, left: 3,
                                            fontSize: 12, fontWeight: "bold",
                                            color: labelColor, userSelect: "none"
                                        }}>
                                            {rankLabel}
                                        </span>
                                    )}
                                    {showFile && (
                                        <span style={{
                                            position: "absolute", bottom: 2, right: 3,
                                            fontSize: 12, fontWeight: "bold",
                                            color: labelColor, userSelect: "none"
                                        }}>
                                            {fileLabel}
                                        </span>
                                    )}
                                    {imgSrc && (
                                        <img
                                            src={imgSrc}
                                            alt={cell}
                                            draggable={true}
                                            onDragStart={(e) => handleDragStart(e, rowIndex, colIndex)}
                                            style={{ width: "100%", height: "100%", cursor: "grab" }}
                                        />
                                    )}
                                </div>
                            )
                        })}
                    </div>
                ))}

                {pendingPromotion && (
                    <>
                        <div style={{position: "absolute", inset: 0, backgroundColor: "rgba(0,0,0,0.4)"}}></div>
                        <div
                            style={{
                                position: "absolute", top: "50%", left: "50%",
                                transform: "translate(-50%, -50%)",
                                display: "flex", gap: 8, padding: 12,
                                backgroundColor: "#fff", borderRadius: 8,
                                boxShadow: "0 4px 16px rgba(0,0,0,0.3)"
                            }}
                        >
                            {promotionChoices.map((type) => {
                                const displayChar = pendingPromotion.isWhite ? type.toUpperCase() : type
                                return (
                                    <img
                                        key={type}
                                        src={getPieceImage(displayChar)}
                                        alt={displayChar}
                                        onClick={() => choosePromotion(type)}
                                        style={{ width: CELL_SIZE, height: CELL_SIZE, cursor: "pointer" }}
                                    />
                                )
                            })}
                        </div>
                    </>
                )}

                {gameState !== GameState.ONGOING && (
                    <>
                        <div style={{position: "absolute", inset: 0, backgroundColor: "rgba(0,0,0,0.4)"}}></div>
                        <div
                            style={{
                                position: "absolute", top: "50%", left: "50%",
                                transform: "translate(-50%, -50%)",
                                display: "flex", gap: 8, padding: 12,
                                backgroundColor: "#fff", borderRadius: 8,
                                boxShadow: "0 4px 16px rgba(0,0,0,0.3)"
                            }}
                        >
                            <div>Game ended: {gameState}</div>
                        </div>
                    </>
                )}
            </div>
        </>
    )
}

export default DrawBoard