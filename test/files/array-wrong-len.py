from typing import Optional

from testcase import Code
from errors import TestError, Severity
from output import KantanOutput


class Test(Code):
    def check(self, output: KantanOutput) -> Optional[TestError]:
        if len(output.errors) != 3:
            msg = 'expected 3 errors, but got {}'.format(len(output.errors))
            return self.create_error(msg)

        error = output.errors[0]
        expected_rsn = "Invalid operator for types \\'[7]i8\\' and \\'[6]i8\\'"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 6:
            return self.create_error('wrong line')

        if error.col != 21:
            print(error.col)
            return self.create_error('wrong column')

        error = output.errors[1]
        expected_rsn = "Array length must be an int literal or \\'_\\'"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 7:
            return self.create_error('wrong line')

        if error.col != 14:
            print(error.col)
            return self.create_error('wrong column')

        error = output.errors[2]
        expected_rsn = "Variable was declared with type \\'[6]u8\\', but initialized with type \\'string\\'"
        actual_rsn = error.reason
        if expected_rsn not in actual_rsn:
            msg = 'wrong reason, expected <{}>, but got {}'.format(expected_rsn, actual_rsn)
            return self.create_error(msg)

        if error.line != 8:
            return self.create_error('wrong line')

        if error.col != 21:
            print(error.col)
            return self.create_error('wrong column')

        return None
