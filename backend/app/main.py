from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from app.routers import auth
from app.routers import chess

from contextlib import asynccontextmanager
from app.database import create_db_and_tables

@asynccontextmanager
async def lifespan(app: FastAPI):
    """ Setup the database """
    create_db_and_tables()
    yield


app = FastAPI(title="Blunder API", lifespan=lifespan)
app.include_router(auth.router, prefix="/api/auth")
app.include_router(chess.router, prefix="/api/chess")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/api/health")
def health():
    return {"status": "ok"}