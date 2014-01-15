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
            

class MyErrIO(io.IOBase):
    def __init__(self):
        super().__init__()
        try:
            import _myModule
            self.write = _myModule.myErrPrint
        except ImportError:
            self.write = sys.__stderr__.write 

sys.stdout = MyIO();
sys.stderr = MyErrIO();

def func(s):
    a = compile(s, "", "exec")
    return exec(a)

if __name__ == "__main__":
    pass
