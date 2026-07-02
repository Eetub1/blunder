from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

from app.chess.board import parse_fen, apply_move, to_fen, flip_turn, algebraic_to_indices, indices_to_algebraic
from app.chess.moves import calculate_moves
from app.chess.rules import calculate_legal_moves, is_move_legal, is_stalemate_or_checkmate, GameState

app = FastAPI(title="Blunder API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/api/health")
def health():
    return {"status": "ok"}


class MoveRequest(BaseModel):
    fen: str
    from_square: str # algebraic notation, i.e. "e4"
    to_square: str
    promotion: str | None = None # for example, "Q" means that a pawn can be promoted to white queen

class MoveResponse(BaseModel):
    fen: str
    legal: bool
    stalemate: bool | None = None
    checkmate: bool | None = None

class ValidSquaresRequest(BaseModel):
    fen: str
    square: str

class ValidSquaresResponse(BaseModel):
    valid_squares: list[str]


@app.post("/api/move", response_model=MoveResponse)
def make_move(req: MoveRequest):
    original_fen = req.fen
    fen_parts = req.fen.split(" ")
    fen_placement = fen_parts[0]
    fen_tail = fen_parts[1:]
    en_passant_target_square = fen_parts[3]

    try:
        board = parse_fen(fen_placement)
        is_whites_turn = fen_parts[1].lower() == "w"
        from_row, from_col = algebraic_to_indices(req.from_square)
        to_row, to_col = algebraic_to_indices(req.to_square)
        moving_piece = board[from_row][from_col].lower()

        # validating the move
        legal_targets = calculate_legal_moves(board, req.from_square, is_whites_turn, en_passant_target_square)
        if req.to_square not in legal_targets:
            return MoveResponse(fen=original_fen, legal=False)

        # we apply the move to the board (Just naively moving the piece from start to end)
        board = apply_move(board, req.from_square, req.to_square)

        # if the move is a promotion, put the correct promoted piece on board
        if req.promotion:
            row, col = algebraic_to_indices(req.to_square)
            board[row][col] = req.promotion

        # en passant logic
        # clearing the piece captured by en passant. This has to be handled here
        if moving_piece == "p" and req.to_square == en_passant_target_square:
            board[from_row][to_col] = ""

        # if the pawn moved 2 squares, there is en passant available and we mark it to 
        # the fen string
        if moving_piece == "p" and abs(to_row - from_row) == 2:
            skipped_row = (to_row + from_row) // 2
            fen_tail[2] = indices_to_algebraic([skipped_row, from_col])
        else:
            fen_tail[2] = "-"

        # rebuilding the fen string
        fen = to_fen(board) + " " + " ".join(fen_tail)
        fen = flip_turn(fen)

        # has game ended
        game_result = is_stalemate_or_checkmate(board, not is_whites_turn)
        stalemate = game_result == GameState.STALEMATE
        checkmate = game_result == GameState.CHECKMATE

    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))

    return MoveResponse(fen=fen, legal=True, stalemate=stalemate, checkmate=checkmate)


@app.post("/api/moves", response_model=ValidSquaresResponse)
def get_valid_squares(req: ValidSquaresRequest):
    try:
        fen_parts = req.fen.split(" ")
        board = parse_fen(fen_parts[0])
        is_whites_turn = fen_parts[1].lower() == "w"
        en_passant = fen_parts[3]

        valid_squares = calculate_legal_moves(board, req.square, is_whites_turn, en_passant)
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))
    
    return ValidSquaresResponse(valid_squares=valid_squares)