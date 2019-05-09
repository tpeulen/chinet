import utils
import os
import unittest
import sys

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_port_init_value(self):
        """Test chinet Port class set_value and get_value"""
        value = 23.0
        p1 = cn.Port()
        p1.set_value(value)
        p2 = cn.Port(value)
        self.assertEqual(p1.get_value(), p2.get_value())

    def test_port_init_array(self):
        """Test chinet Port class set_value and get_value"""
        value = (1, 2, 3, 5, 8, 13)
        p1 = cn.Port()
        p1.set_array(value)
        p2 = cn.Port(value)
        self.assertEqual(p1.get_array(), p2.get_array())

    def test_set_get_value(self):
        """Test chinet Port class set_value and get_value"""
        value = 23.0
        port = cn.Port(value)
        self.assertEqual(port.get_value(), value)

    def test_set_get_array(self):
        """Test chinet Port class set_array and get_array"""
        value = (1, 2, 3, 5, 8, 13)
        port = cn.Port()
        port.set_array(value)
        self.assertEqual(port.get_array(), value)

    def test_port_link_value(self):
        value1 = 12
        value2 = 6
        p1 = cn.Port(value1)
        p2 = cn.Port(value2)

        self.assertEqual(p1.get_value(), value1)
        self.assertEqual(p2.get_value(), value2)
        self.assertNotEqual(p1.get_value(), p2.get_value())

        p2.link(p1)
        self.assertEqual(p1.get_value(), p2.get_value())

        p2.unlink()
        self.assertEqual(p2.get_value(), value2)

    def test_port_fixed(self):
        p1 = cn.Port(12)

        p1.set_fixed(True)
        self.assertEqual(p1.is_fixed(), True)

        p1.set_fixed(False)
        self.assertEqual(p1.is_fixed(), False)


if __name__ == '__main__':
    unittest.main()