from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

from app.chess.board import parse_fen, apply_move, to_fen

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
    from_square: str # these are both in algebraic notation, so "e4"
    to_square: str

class MoveResponse(BaseModel):
    fen: str
    legal: bool


@app.post("/api/move", response_model=MoveResponse)
def make_move(req: MoveRequest):

    board = parse_fen(req.fen)
    board = apply_move(board, req.from_square, req.to_square) # Move should be validated before applying TODO
    fen = to_fen(board)

    # also right now the fen string only contains the placement. The other part needs to be dealt with too!
    # right now every move is true
    return MoveResponse(fen=fen, legal=True)