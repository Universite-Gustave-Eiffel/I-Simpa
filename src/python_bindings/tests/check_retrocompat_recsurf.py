#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import os
import sys
sys.path.append(os.getcwd())
import libsimpa as ls
import unittest




class TestImportSurfaceReceiver(unittest.TestCase):

    def test_import(self):        
        dat=ls.rsurf_data()
        resourceFile = os.path.join(os.path.dirname(os.path.realpath(__file__)), "rs_cut.csbin")
        self.assertTrue(os.path.exists(resourceFile))
        self.assertTrue(ls.rsurf_io.Load(resourceFile,dat))
        self.assertEqual(dat.GetRsCount(), 1)
        self.assertEqual(dat.GetRsFaceCount(0), 1248)
        self.assertEqual(dat.GetRsName(0).decode('cp1252'), u"Récepteur coupe") #encoding will be utf-8 on all platforms
        self.assertEqual(dat.GetRsXmlId(0), 920)


if __name__ == '__main__':
    unittest.main()


