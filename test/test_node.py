import utils
import os
import unittest
import sys
import json

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_node_init(self):
        ports = {
            'inA': cn.Port(7.0),
            'inB': cn.Port(13.0),
            'outC': cn.Port(0.0, False, True)
        }
        node_with_ports = cn.Node(ports)

        self.assertEqual(node_with_ports.get_input_ports().keys(), ['inA', 'inB'])

        values = [v.get_value() for v in node_with_ports.get_ports().values()]
        self.assertEqual(values, [7.0, 13.0, 0.0])

    def test_node_name(self):
        node = cn.Node()

        portA_name = "inA"
        portA = cn.Port(17)
        node.add_input_port(portA_name, portA)

        portB_name = "inB"
        portB = cn.Port(23)
        node.add_input_port(portB_name, portB)

        portOut1 = cn.Port()
        node.add_output_port("outA", portOut1)

        self.assertEqual(node.get_name(), ", : (inA,inB,)->(outA,)")

        node.set_callback("multiply", "C")
        self.assertEqual(node.get_name(), ", multiply: (inA,inB,)->(outA,)")

        node.set_name("NodeName")
        self.assertEqual(node.get_name(), "NodeName, multiply: (inA,inB,)->(outA,)")

    def test_node_ports(self):
        node = cn.Node()

        portA_name = "portA"
        portA = cn.Port(17)
        node.add_input_port(portA_name, portA)

        portB_name = "portB"
        portB = cn.Port(23)
        node.add_output_port(portB_name, portB)

        self.assertEqual(portA, node.get_input_port(portA_name))
        self.assertEqual(portB, node.get_output_port(portB_name))

    def test_node_C_RTTR_callback(self):
        """Test chinet RTTR C callbacks"""
        node = cn.Node()

        inA = cn.Port([2., 3., 4.])
        node.add_input_port("inA", inA)

        outA = cn.Port()
        node.add_output_port("outA", outA)

        node.set_callback("multiply", "C")

        node.evaluate()
        self.assertEqual((inA.get_value().prod() == outA.get_value()).all(), True)

    def test_node_C_RTTR_callback_2(self):
        """Test chinet RTTR C callbacks"""
        node = cn.Node(
            {
                "inA": cn.Port([2., 3., 4.], False, False),
                "outA": cn.Port(0, False, True)
            }
        )
        node.set_callback("multiply", "C")
        node.evaluate()
        outA = node.get_ports()["outA"]
        inA = node.get_ports()["inA"]

        self.assertEqual((inA.get_value().prod() == outA.get_value()).all(), True)

    def test_node_python_callback(self):
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

        portIn1 = cn.Port(55)
        node.add_input_port("portA", portIn1)

        portIn2 = cn.Port(2)
        node.add_input_port("portB", portIn2)

        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        cb = NodeCallback()
        node.set_callback(cb)
        node.evaluate()

        self.assertEqual(portOut1.get_value(), portIn1.get_value() * portIn2.get_value())

    def test_node_write_to_db(self):
        db_dict = {
            'uri_string': "mongodb://localhost:27017",
            'db_string': "chinet",
            'app_string': "chisurf",
            'collection_string': "test_collection"
        }

        node = cn.Node(
            {
                'portA': cn.Port(55),
                'portB': cn.Port(2),
                'portC': cn.Port()
            }
        )
        node.set_callback("multiply", "C")
        self.assertEqual(node.connect_to_db(**db_dict), True)
        self.assertEqual(node.write_to_db(), True)

    def test_node_restore_from_db(self):
        db_dict = {
            'uri_string': "mongodb://localhost:27017",
            'db_string': "chinet",
            'app_string': "chisurf",
            'collection_string': "test_collection"
        }

        node = cn.Node(
            {
                'portA': cn.Port(13.0),
                'portB': cn.Port(2.0),
                'portC': cn.Port(1.0)
            }
        )
        node.set_callback("multiply", "C")
        node.connect_to_db(**db_dict)
        node.write_to_db()

        node_restore = cn.Node()
        node_restore.connect_to_db(**db_dict)
        node_restore.read_from_db(node.get_oid())

        dict_restore = json.loads(node_restore.get_json())
        dict_original = json.loads(node.get_json())

        self.assertEqual(dict_restore, dict_original)


if __name__ == '__main__':
    unittest.main()