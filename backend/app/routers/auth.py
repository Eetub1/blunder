from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from sqlmodel import Session, select

from app.database import get_session
from app.models.user import User
from app.auth.helpers import verify_password, hash_password
from app.auth.security import create_access_token

router = APIRouter()

class SignUpRequest(BaseModel):
    username: str
    password: str

class LoginRequest(BaseModel):
    username: str
    password: str


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

    user = session.exec(select(User).where(User.username == username)).first()

    # verify password
    if not user or not verify_password(password, user.hashed_password):
        raise HTTPException(status_code=401, detail="Invalid username or password")

    # create a token when user logs in
    token = create_access_token(user.id)
    return {
        "access_token": token,
        "token_type": "bearer",
        "user": {
            "username": user.username,
            "id": user.id
        }
    }