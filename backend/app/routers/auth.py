from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from sqlmodel import Session, select
from app.database import get_session
from app.models.user import User

import bcrypt

router = APIRouter()

class TempResponse(BaseModel):
    message: str

class SignUpRequest(BaseModel):
    username: str
    password: str

class LoginRequest(BaseModel):
    username: str
    password: str


def verify_password(password: str, hashed: str) -> bool:
    return bcrypt.checkpw(password.encode('utf-8'), hashed.encode('utf-8'))


def hash_password(password: str):
    salt = bcrypt.gensalt()
    hashed = bcrypt.hashpw(password.encode('utf-8'), salt)
    return hashed.decode('utf-8')


@router.post("/signup")
def sign_up(req: SignUpRequest, session: Session = Depends(get_session)):
    username, password = req.username, req.password

    existing = session.exec(select(User).where(User.username == username)).first()
    if existing:
        raise HTTPException(status_code=400, detail="Username already taken")

    user = User(username=username, hashed_password=hash_password(password))
    session.add(user)
    session.commit()
    session.refresh(user)
    return {"id": user.id, "username": user.username}


@router.post("/login")
def login(req: LoginRequest, session: Session = Depends(get_session)):
    username, password = req.username, req.password

    # get the correct user based on username
    user = session.exec(select(User).where(User.username == username)).first()

    # verify password
    if not user or not verify_password(password, user.hashed_password):
        raise HTTPException(status_code=401, detail="Invalid username or password")
    return {"id": user.id, "username": user.username}
