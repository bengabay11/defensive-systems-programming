from dataclasses import dataclass

from sqlalchemy import UUID, String, Column, LargeBinary, Date

from dal.models.base import Base


class Client(Base):
    __tablename__ = 'clients'
    
    id = Column(UUID, primary_key=True)
    name = Column(String)
    public_key = Column(LargeBinary)
    last_seen = Column(Date)
    aes_key = Column(LargeBinary)
