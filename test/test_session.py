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
        n1 = cn.Node(
            {
                'portA': cn.Port(1),
                'portB': cn.Port(2),
                'portC': cn.Port(3)
            },
        )
        n2 = cn.Node(
            {
                'inA': cn.Port(5),
                'inB': cn.Port(7),
                'outA': cn.Port(11, False, True),
                'outB': cn.Port(13, False, True)
            }
        )

        s1 = cn.Session()
        s1.add_node("nodeA", n1)
        s1.add_node("nodeB", n2)

        nodes = {
            "nodeA": n1,
            "nodeB": n2
        }
        s2 = cn.Session(nodes)

        na1 = s1.get_nodes()['nodeA']
        na2 = s2.get_nodes()['nodeA']
        na1.set_name("new name")

        # the nodes are references
        self.assertEqual(na1.get_name(), na2.get_name())

    def test_read_template(self):
        import chinet as cn

        template_file = "./test/inputs/session_template.json"

        json_string = ""
        with open(template_file, 'r') as fp:
            json_string = fp.read()

        s = cn.Session()
        s.read_session_template(json_string)
        print(s.get_nodes().keys())

        print(
            s.get_nodes()[s.get_nodes().keys()[0]].get_input_ports().keys()
        )
        print(
            [d.get_value() for d in s.get_nodes()[s.get_nodes().keys()[0]].get_input_ports().values()]
        )



if __name__ == '__main__':
    unittest.main()