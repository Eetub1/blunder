from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

from app.chess.board import parse_fen, apply_move, to_fen, flip_turn, algebraic_to_indices, indices_to_algebraic
from app.chess.rules import calculate_legal_moves, is_stalemate_or_checkmate, GameState

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
    game_state: GameState | GameState = GameState.ONGOING
    from_square: str | str = "" # these are empty if the move was not valid
    to_square: str | str = ""

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
    castling_rights = fen_parts[2]
    en_passant_target_square = fen_parts[3]

    try:
        board = parse_fen(fen_placement)
        is_whites_turn = fen_parts[1].lower() == "w"
        from_row, from_col = algebraic_to_indices(req.from_square)
        to_row, to_col = algebraic_to_indices(req.to_square)
        moving_piece = board[from_row][from_col].lower()

        # validating the move
        legal_targets = calculate_legal_moves(board, req.from_square, is_whites_turn, en_passant_target_square, castling_rights)
        if req.to_square not in legal_targets:
            return MoveResponse(fen=original_fen, legal=False)

        # we apply the move to the board (Just naively moving the piece from start to end)
        board = apply_move(board, req.from_square, req.to_square)

        #============PROMOTION=============================

        # if the move is a promotion, put the correct promoted piece on board
        if req.promotion:
            row, col = algebraic_to_indices(req.to_square)
            board[row][col] = req.promotion

        #============EN PASSANT LOGIC=====================
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

        #============CASTLING==============================
        # moving the rook to the right place, because apply_move only moves the king
        if moving_piece == "k":
            if abs(from_col - to_col) == 2: # move is a castle

                if req.to_square == "g1":
                    board = apply_move(board, "h1", "f1")
                elif req.to_square == "c1":
                    board = apply_move(board, "a1", "d1")
                elif req.to_square == "g8":
                    board = apply_move(board, "h8", "f8")
                elif req.to_square == "c8":
                    board = apply_move(board, "a8", "d8")

        # removing the castling rights
        castling_part = fen_tail[1]

        if moving_piece == "k":
            if is_whites_turn:
                castling_part = castling_part.replace("K", "").replace("Q", "")
            else:
                castling_part = castling_part.replace("k", "").replace("q", "")

        if moving_piece == "r":
            if req.from_square == "a1": castling_part = castling_part.replace("Q", "")
            elif req.from_square == "h1": castling_part = castling_part.replace("K", "")
            elif req.from_square == "a8": castling_part = castling_part.replace("q", "")
            elif req.from_square == "h8": castling_part = castling_part.replace("k", "")

        # fixes an edge case where a rook is captured in its homesquare
        if req.to_square == "a1": castling_part = castling_part.replace("Q", "")
        elif req.to_square == "h1": castling_part = castling_part.replace("K", "")
        elif req.to_square == "a8": castling_part = castling_part.replace("q", "")
        elif req.to_square == "h8": castling_part = castling_part.replace("k", "")

        # empty castling right part should be '-' 
        fen_tail[1] = castling_part if castling_part != "" else "-"

        # rebuilding the fen string
        fen = to_fen(board) + " " + " ".join(fen_tail)
        fen = flip_turn(fen)

        # has game ended
        game_result = is_stalemate_or_checkmate(board, not is_whites_turn)
        print(game_result)
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))

    return MoveResponse(fen=fen, legal=True, game_state=game_result, from_square=req.from_square, to_square=req.to_square)


@app.post("/api/moves", response_model=ValidSquaresResponse)
def get_valid_squares(req: ValidSquaresRequest):
    try:
        fen_parts = req.fen.split(" ")
        board = parse_fen(fen_parts[0])
        is_whites_turn = fen_parts[1].lower() == "w"
        castling_rights = fen_parts[2]
        en_passant = fen_parts[3]

        valid_squares = calculate_legal_moves(board, req.square, is_whites_turn, en_passant, castling_rights)
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))
    
    return ValidSquaresResponse(valid_squares=valid_squares)