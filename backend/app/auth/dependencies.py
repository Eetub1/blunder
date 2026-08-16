from fastapi import Depends, HTTPException, status
from fastapi.security import OAuth2PasswordBearer
from sqlmodel import Session

from app.database import get_session
from app.models.user import User
from app.auth.security import decode_access_token

# tokenUrl is the path clients hit to get a token
oauth2_scheme = OAuth2PasswordBearer(tokenUrl="api/auth/login")


def get_current_user(token: str = Depends(oauth2_scheme), session: Session = Depends(get_session),) -> User:
    """ Try to get the user from the token """

    credentials_error = HTTPException(
        status_code=status.HTTP_401_UNAUTHORIZED,
        detail="Could not validate credentials",
        headers={"WWW-Authenticate": "Bearer"},
    )

    user_id = decode_access_token(token)
    if user_id is None:
        raise credentials_error

    user = session.get(User, user_id)
    if user is None:
        raise credentials_error

    return user
