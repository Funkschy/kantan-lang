from typing import List
from testcase import Code


class Test(Code):
    def files(self) -> List[str]:
        return list(map(lambda f: self.relative_to(f), [
            'unqualified-imports/main.kan',
            'unqualified-imports/other1.kan',
            'unqualified-imports/other2.kan',
            'unqualified-imports/other3.kan',
        ]))
