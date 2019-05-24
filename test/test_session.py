import utils
import os
import unittest
import numpy as np

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

    def test_read_session_template(self):
        template_file = "./inputs/session_template.json"

        with open(template_file, 'r') as fp:
            json_string = fp.read()

            s = cn.Session()
            s.read_session_template(json_string)

            nodeA = s.get_nodes()[s.get_nodes().keys()[0]]
            nodeB = s.get_nodes()[s.get_nodes().keys()[1]]

            # test reading of nodes
            self.assertListEqual(
                s.nodes.keys(),
                ['nodeA', 'nodeB']
            )

            # test reading of ports
            self.assertListEqual(
                nodeA.get_input_ports().keys(),
                ['inA']
            )

            # test reading of port values
            self.assertListEqual(
                list(np.hstack([d.get_value() for d in nodeA.get_input_ports().values()])),
                [1., 2., 3., 4., 5.]
            )

            # test evaluate
            nodeA.evaluate()

            nodeA_inA = nodeA.get_port("inA")
            nodeB_inA = nodeB.get_port("inA")

            # test links
            self.assertListEqual(
                list(nodeA_inA.get_value()),
                list(nodeB_inA.get_value())
            )

            v = list(np.hstack([d.get_value() for d in nodeA.get_output_ports().values()]))
            self.assertListEqual(v, [1., 2., 3., 4., 5.])


if __name__ == '__main__':
    unittest.main()