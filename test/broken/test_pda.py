import utils
import os
import unittest
import json
import numpy as np

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_pda(self):
        """Test chinet Port class set_value and get_value"""
        import pylab as p

        n_max = 100

        pda = cn.Pda()
        pda.setBg(1.5)
        pda.setBr(10.5)
        pda.setNmax(n_max)

        pf = np.ones(100, dtype=np.float)
        pda.setPF(pf)
        pda.append(1, 0.1)
        pda.append(1, 0.9)
        pda.evaluate()
        sgsr = pda.getSgSr().reshape(n_max, n_max)
        p.imshow(sgsr)
        p.show()

        self.assertEqual(p1.get_value(), p2.get_value())
        self.assertEqual(p3.is_fixed(), True)
        self.assertEqual(p4.is_fixed(), False)

        # check linking
        p5 = cn.Port(v2)
        p5.link(p4)
        self.assertEqual(p5.get_value(), p4.get_value())
