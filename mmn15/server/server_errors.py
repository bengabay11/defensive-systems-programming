import abc

from protocol import ResponseCodes

class BaseServerError(Exception, abc.ABC):
    @staticmethod
    @abc.abstractmethod
    def error_code() -> int:
        return ResponseCodes.GENERAL_SERVER_ERROR


class RegistrationFailError(BaseServerError):
    def __init__(self, client_name: str) -> None:
        self._client_name = client_name
        message = f"Registration failed, client with name '{client_name}' already registered."
        super().__init__(message)

    @staticmethod
    def error_code() -> int:
        return ResponseCodes.REGISTRATION_FAIL