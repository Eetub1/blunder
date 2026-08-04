from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from sqlmodel import Session, select
from app.database import get_session
from app.models.user import User

router = APIRouter()

class TempResponse(BaseModel):
    message: str

class SignUpRequest(BaseModel):
    username: str
    password: str


def hash_password(password):
    pass


@router.post("/signup")
def sign_up(req: SignUpRequest, session: Session = Depends(get_session)):
    username, password = req.username, req.password

    existing = session.exec(select(User).where(User.username == username)).first()
    if existing:
        raise HTTPException(status_code=400, detail="Username already taken")

    user = User(username=username, hashed_password=hash_password(password))  # see note below
    session.add(user)
    session.commit()
    session.refresh(user)
    return {"id": user.id, "username": user.username}


@router.post("login")
def login():
    return TempResponse(message="Moro login osasta")