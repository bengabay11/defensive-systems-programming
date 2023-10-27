from dataclasses import dataclass

from sqlalchemy import String, Column, LargeBinary, DateTime
from sqlalchemy_utils import UUIDType

from dal.models.base import Base


class Client(Base):
    __tablename__ = 'clients'
    
    id = Column(UUIDType, primary_key=True)
    name = Column(String)
    public_key = Column(LargeBinary)
    last_seen = Column(DateTime)
    aes_key = Column(LargeBinary)
