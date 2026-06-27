from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

app = FastAPI(title="WannaMate API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/api/health")
def health():
    return {"status": "ok"}


# --- the move contract (stubbed for now) ---
class MoveRequest(BaseModel):
    fen: str
    from_square: str   # algebraic, e.g. "e2"
    to_square: str     # algebraic, e.g. "e4"

class MoveResponse(BaseModel):
    fen: str
    legal: bool

@app.post("/api/move", response_model=MoveResponse)
def make_move(req: MoveRequest):
    # TODO: parse the FEN, apply the move, re-serialize — in app/chess/board.py.
    # Then add legality. For now just echo so the contract is real and testable.
    return MoveResponse(fen=req.fen, legal=True)