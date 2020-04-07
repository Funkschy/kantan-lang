from typing import Optional

from testcase import Code
from errors import TestError, Severity
from output import KantanOutput


class Test(Code):
    def check(self, output: KantanOutput) -> Optional[TestError]:
        if len(output.errors) != 3:
            msg = 'expected 1 errors, but got {}'.format(len(output.errors))
            return self.create_error(msg)

        error = output.errors[0]
        expected_rsn = "Importing outside the global scope is currently not allowed, this may change in a future Kantan release"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 2:
            return self.create_error('wrong line')

        if error.col != 13:
            print(error.col)
            return self.create_error('wrong column')

        error = output.errors[1]
        expected_rsn = "Declaring Functions outside the global scope is currently not allowed, this may change in a future Kantan release"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 3:
            return self.create_error('wrong line')

        if error.col != 9:
            print(error.col)
            return self.create_error('wrong column')

        error = output.errors[2]
        expected_rsn = "Declaring Structs outside the global scope is currently not allowed, this may change in a future Kantan release"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 6:
            return self.create_error('wrong line')

        if error.col != 10:
            print(error.col)
            return self.create_error('wrong column')

        return None
