from dataclasses import dataclass

from sqlalchemy import UUID, Integer, String, Column, Boolean

from dal.models.base import Base


class File(Base):
    __tablename__ = 'files'
    
    id = Column(UUID, primary_key=True)
    file_name = Column(String, primary_key=True)
    path_name = Column(String)
    verified = Column(Boolean, default=False)
