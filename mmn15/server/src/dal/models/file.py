from sqlalchemy import String, Column, Boolean
from sqlalchemy_utils import UUIDType

from dal.models.base import Base


class File(Base):
    __tablename__ = 'files'
    
    id = Column(UUIDType, primary_key=True)
    file_name = Column(String, primary_key=True)
    path_name = Column(String)
    verified = Column(Boolean, default=False)
