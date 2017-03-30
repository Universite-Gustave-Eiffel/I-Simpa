from __future__ import print_function
import os
import sys
sys.path.append(os.getcwd())
import libsimpa as ls

import unittest




class TestMathMethods(unittest.TestCase):

    def test_init(self):
        v = ls.vec3(1.2, 1.5, -2.8)


if __name__ == '__main__':
    unittest.main()