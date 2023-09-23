from typing import Type, Any

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from dal.models.base import Base


class DBConnection(object):
    def __init__(self) -> None:
        self._connection = None
        self._session_class = None

    def connect(self, protocol: str, database: str) -> None:
        url = f"{protocol}:///{database}"
        engine = create_engine(url)
        session_maker = sessionmaker(bind=engine)
        self._session = session_maker()
        self._connection = engine.connect()

    def select(self, model: Type[Base], filters: tuple = ()) -> list:
        query = self._session.query(model).filter(*filters)
        return query.all()

    def insert(self, row: Base) -> None:
        self._session.add(row)
        self._session.commit()

    def update(self, model: Type[Base], filters: tuple, column: Any, new_value: Any) -> None:
        row = self._session.query(model).filter(*filters).one()
        setattr(row, column, new_value)
        self._session.commit()

    def delete(self, model: Type[Base], filters: tuple) -> None:
        row = self._session.query(model).filter(*filters).one()
        self._session.delete(row)
        self._session.commit()

    def close(self) -> None:
        self._session.close()
        self._connection.close()
