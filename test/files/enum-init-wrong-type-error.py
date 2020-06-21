from typing import Optional

from testcase import Code
from errors import TestError, Severity
from output import KantanOutput


class Test(Code):
    def check(self, output: KantanOutput) -> Optional[TestError]:
        if len(output.errors) != 1:
            msg = 'expected 1 error, but got {}'.format(len(output.errors))
            return self.create_error(msg)


        error = output.errors[0]
        expected_rsn = "Invalid enum start value type \\'string\\' in enum \\'enum-init-wrong-type-error.Error\\'"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 2:
            return self.create_error('wrong line')

        if error.col != 14:
            print(error.col)
            return self.create_error('wrong column')

        return None
