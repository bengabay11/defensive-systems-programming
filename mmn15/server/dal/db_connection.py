from functools import wraps
from typing import Type, Any

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, Session

from dal.models.base import Base


def with_session(expire_on_commit=True):
    def decorator(func):
        @wraps(func)
        def wrapper(self, *args, **kwargs):
            if self._connection is None:
                raise Exception("Database connection not established. Call connect() first.")
            session = self.session_maker(expire_on_commit=expire_on_commit)
            try:
                result = func(self, session, *args, **kwargs)
                session.commit()
                return result
            except Exception as e:
                session.rollback()
                raise e
            finally:
                session.close()
        return wrapper
    return decorator


class DBConnection(object):
    def __init__(self) -> None:
        self._connection = None
        self._session_class = None

    def connect(self, protocol: str, database: str) -> None:
        url = f"{protocol}:///{database}"
        engine = create_engine(url)
        self.session_maker = sessionmaker(bind=engine)
        self._connection = engine.connect()

    @with_session(expire_on_commit=False)
    def select(self, session: Session, model: Type[Base], filters: tuple = ()) -> list:
        query = session.query(model).filter(*filters)
        return query.all()

    @with_session(expire_on_commit=False)
    def insert(self, session: Session, row: Base) -> None:
        session.add(row)

    @with_session()
    def update(self, session: Session, model: Type[Base], filters: tuple, column: Any, new_value: Any) -> None:
        row = session.query(model).filter(*filters).first()
        setattr(row, column, new_value)

    @with_session()
    def delete(self, session: Session, model: Type[Base], filters: tuple) -> None:
        row = session.query(model).filter(*filters).one()
        session.delete(row)

    def close(self) -> None:
        self._connection.close()