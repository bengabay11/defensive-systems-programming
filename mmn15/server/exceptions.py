

class EmptyRequestHeaderException(Exception):
    def __init__(self) -> None:
        message = f"Received empty request header."
        super().__init__(message)


class UnsupportedRequestCodeException(Exception):
    def __init__(self, request_code: int) -> None:
        self._request_code = request_code
        message = f"Unsupported request code - '{self._request_code}'"
        super().__init__(message)
