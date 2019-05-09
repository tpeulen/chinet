import utils
import os
import unittest
import sys

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_C_RTTR_callback(self):
        """Test chinet RTTR C callbacks"""
        node = cn.Node()

        portIn1 = cn.Port()
        portIn1.set_value(55)
        node.add_input_port("portA", portIn1)

        portIn2 = cn.Port()
        portIn2.set_value(2)
        node.add_input_port("portB", portIn2)

        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        node.set_callback("multiply", "C")

        node.update()

        self.assertEqual(portOut1.get_value(), portIn1.get_value() * portIn2.get_value())

    def test_python_callback(self):
        """Test chinet Node class python callbacks"""

        class NodeCallback(cn.NodeCallback):

            def __init__(self, *args, **kwargs):
                cn.NodeCallback.__init__(self, *args, **kwargs)

            def run(self, inputs, outputs):
                mul = 1.0
                for key in inputs:
                    mul *= inputs[key].get_value()
                outputs["portC"].set_value(mul)

        node = cn.Node()

        portIn1 = cn.Port()
        portIn1.set_value(55)
        node.add_input_port("portA", portIn1)

        portIn2 = cn.Port()
        portIn2.set_value(2)
        node.add_input_port("portB", portIn2)

        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        cb = NodeCallback()
        node.set_callback(cb)
        node.update()
        self.assertEqual(portOut1.get_value(), portIn1.get_value() * portIn2.get_value())


if __name__ == '__main__':
    unittest.main()