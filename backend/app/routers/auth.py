from fastapi import APIRouter
from pydantic import BaseModel

router = APIRouter()

class TempResponse(BaseModel):
    message: str


@router.post("/signup")
def sign_up():
    return TempResponse(message="Moro signup osasta")


@router.post("login")
def login():
    return TempResponse(message="Moro login osasta")