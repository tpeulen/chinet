import utils
import os
import unittest
import sys
import json

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_session_init(self):
        """Test chinet Port class set_value and get_value"""
        n1 = cn.Node(
            {
                'portA': cn.Port(1),
                'portB': cn.Port(2)
            },
            {
                'portC': cn.Port(3)
            }
        )
        n2 = cn.Node(
            {
                'inA': cn.Port(5),
                'inB': cn.Port(7)
            },
            {
                'outA': cn.Port(11),
                'outB': cn.Port(13)
            }
        )

        s1 = cn.Session()
        s1.add_node("nodeA", n1)
        s1.add_node("nodeB", n2)

        s2 = cn.Session(
            {
                'nodeA': n1,
                'nodeB': n2
            }
        )

        na1 = s1.get_nodes()['nodeA']
        na2 = s2.get_nodes()['nodeA']
        na1.set_name("new name")
        # the nodes are references
        self.assertEqual(na1.get_name(), na2.get_name())

        na1 = s1.get_nodes()['nodeA']
        na2 = s2.get_nodes()['nodeA']


if __name__ == '__main__':
    unittest.main()