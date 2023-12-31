from functools import wraps
import logging
from typing import Type, Any

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, Session
from sqlalchemy_utils import database_exists, create_database

from dal.models.base import Base


def with_session(func):
    @wraps(func)
    def wrapper(self, *args, **kwargs):
        if self._connection is None:
            raise Exception("Database connection not established. Call connect() first.")
        session = self.session_maker()
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


class DBConnection(object):
    def __init__(self) -> None:
        self._connection = None
        self._session_class = None

    def connect(self, protocol: str, database: str) -> None:
        url = f"{protocol}:///{database}"
        engine = create_engine(url)
        if not database_exists(engine.url):
            logging.info(f"Database not exist at - '{url}'. Creating a new one")
            create_database(url)
            Base.metadata.create_all(engine)
        self.session_maker = sessionmaker(bind=engine)
        self._connection = engine.connect()

    @with_session
    def select(self, session: Session, model: Type[Base], filters: tuple = ()) -> list:
        query = session.query(model).filter(*filters)
        rows = query.all()
        for row in rows:
            session.expunge(row)
        return rows


    @with_session
    def insert(self, session: Session, row: Base) -> None:
        session.add(row)

    @with_session
    def update(self, session: Session, model: Type[Base], filters: tuple, column: Any, new_value: Any) -> None:
        row = session.query(model).filter(*filters).one()
        setattr(row, column, new_value)

    @with_session
    def delete(self, session: Session, model: Type[Base], filters: tuple) -> None:
        row = session.query(model).filter(*filters).one()
        session.delete(row)

    def close(self) -> None:
        self._connection.close()
