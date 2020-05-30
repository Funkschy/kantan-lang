from typing import Optional

from testcase import Code
from errors import TestError, Severity
from output import KantanOutput


class Test(Code):
    def check(self, output: KantanOutput) -> Optional[TestError]:
        if len(output.errors) != 2:
            msg = 'expected 2 errors, but got {}'.format(len(output.errors))
            return self.create_error(msg)

        error = output.errors[0]
        expected_rsn = "Invalid type for condition. Expected \\'bool\\', but got \\'string\\'"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 3:
            return self.create_error('wrong line')

        if error.col != 11:
            print(error.col)
            return self.create_error('wrong column')

        error = output.errors[1]
        expected_rsn = "Invalid type for condition. Expected \\'bool\\', but got \\'i32\\'"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 4:
            return self.create_error('wrong line')

        if error.col != 11:
            print(error.col)
            return self.create_error('wrong column')

        return None
