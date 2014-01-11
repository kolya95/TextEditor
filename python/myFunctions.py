import io
import sys


class MyIO(io.TextIOBase):
    def __init__(self):
        super().__init__()
        try:
            import _myModule

            self.write = _myModule.myPrint
        except ImportError:
            self.write = sys.__stdout__.write 


sys.stdout = MyIO()
sys.stderr = MyIO()

def func(s):
    a = compile(s, "", "exec")
    return exec(a)

if __name__ == "__main__":
    pass