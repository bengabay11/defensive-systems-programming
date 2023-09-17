from dataclasses import dataclass

from sqlalchemy import Integer, String, Column, Boolean

from server.dal.models.base import Base



@dataclass
class File(Base):
    __tablename__ = 'files'
    id = Column(Integer, primary_key=True)
    filename = Column(String)
    path_name = Column(String)
    verified = Column(Boolean, default=False)
