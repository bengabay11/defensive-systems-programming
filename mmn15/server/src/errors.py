import abc
from uuid import UUID

from protocol import ResponseCodes

class BaseServerError(Exception, abc.ABC):
    @abc.abstractmethod
    def error_code(self) -> int:
        return ResponseCodes.GENERAL_SERVER_ERROR


class RegistrationFailError(BaseServerError):
    def __init__(self, client_name: str) -> None:
        self.client_name = client_name
        message = f"Registration failed, client with name '{self.client_name}' already registered."
        super().__init__(message)

    def error_code(self) -> int:
        return ResponseCodes.REGISTRATION_FAIL
    

class LoginFailError(BaseServerError):
    def __init__(self, client_id: UUID, client_name: str) -> None:
        self.client_id = client_id
        self.client_name = client_name
        message = f"ID - '{client_id}' and name - '{client_name}' does not match"
        super().__init__(message)

    def error_code(self) -> int:
        return ResponseCodes.LOGIN_FAIL

        
class ClientNotFoundError(BaseServerError):
    def __init__(self, client_id: UUID) -> None:
        self.client_id = client_id
        message = f"Client with id: '{self.client_name}' not found."
        super().__init__(message)


class UnauthorizedClientError(BaseServerError):
    def __init__(self, requested_client_name: str, db_client_name) -> None:
        self.requested_client_name = requested_client_name
        self.db_client_name = db_client_name
        message = f"Given client name '{requested_client_name}' does not match client name in DB '{db_client_name}'"
        super().__init__(message)


class ClientNotFoundError(BaseServerError):
    def __init__(self, client_id: UUID) -> None:
        self.client_id = client_id
        message = f"Client with id - '{client_id}' not found in the DB"
        super().__init__(message)    


class UnsupportedRequestCodeException(BaseServerError):
    def __init__(self, request_code: int) -> None:
        self._request_code = request_code
        message = f"Unsupported request code - '{self._request_code}'"
        super().__init__(message)
