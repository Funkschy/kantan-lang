from typing import List
from testcase import Code


class Test(Code):
    def files(self) -> List[str]:
        return list(map(lambda f: self.relative_to(f), ['imports/main.kan', 'imports/lib.kan', 'imports/inner/inner.kan']))
