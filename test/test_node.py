import utils
import os
import unittest
import sys
import numpy as np
import json

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class CallbackNodePassOn(cn.NodeCallback):

    def __init__(self, *args, **kwargs):
        cn.NodeCallback.__init__(self, *args, **kwargs)

    def run(self, inputs, outputs):
        outputs["outA"].value = inputs["inA"].value


class NodeCallbackMultiply(cn.NodeCallback):

    def __init__(self, *args, **kwargs):
        cn.NodeCallback.__init__(self, *args, **kwargs)

    def run(self, inputs, outputs):
        mul = 1.0
        for key in inputs:
            mul *= inputs[key].value
        outputs["portC"].value = mul


class Tests(unittest.TestCase):

    def test_node_init(self):
        node_with_ports = cn.Node(
            {
                'inA': cn.Port(7.0),
                'inB': cn.Port(13.0),
                'outC': cn.Port(
                    value=0.0,
                    fixed=False,
                    is_output=True
                )
            }
        )
        self.assertEqual(
            node_with_ports.get_input_ports().keys(),
            ['inA', 'inB']
        )
        values = np.hstack(
            [v.value for v in node_with_ports.ports.values()]
        )
        self.assertEqual(
            np.allclose(
                values,
                np.array([7.0, 13.0, 0.0])
            ),
            True
        )

    def test_node_name(self):
        node = cn.Node()

        portA_name = "inA"
        portA = cn.Port(17)
        node.add_input_port(portA_name, portA)

        portB_name = "inB"
        portB = cn.Port(23)
        node.add_input_port(portB_name, portB)

        outA = cn.Port()
        node.add_output_port("outA", outA)

        self.assertEqual(
            node.name,
            ", : (inA,inB,)->(outA,)"
        )

        node.set_callback("multiply_int", "C")
        self.assertEqual(
            node.get_name(),
            ", multiply_int: (inA,inB,)->(outA,)"
        )

        node.set_name("NodeName")
        self.assertEqual(
            node.get_name(),
            'NodeName, multiply_int: (inA,inB,)->(outA,)'
        )

    def test_node_ports(self):
        node = cn.Node()
        portA_name = "portA"
        portA = cn.Port(17)
        node.add_input_port(portA_name, portA)
        portB_name = "portB"
        portB = cn.Port(23)
        node.add_output_port(portB_name, portB)

        self.assertEqual(
            portA,
            node.get_input_port(portA_name)
        )
        self.assertEqual(
            portB,
            node.get_output_port(portB_name)
        )

    def test_node_C_RTTR_callback(self):
        """Test chinet RTTR C callbacks"""
        node = cn.Node()
        inA = cn.Port([2., 3., 4.])
        inB = cn.Port([2., 3., 4.])
        node.add_input_port("inA", inA)
        node.add_input_port("inB", inB)
        outA = cn.Port()
        node.add_output_port("outA", outA)
        node.set_callback("multiply_double", "C")
        node.evaluate()
        self.assertEqual(
            np.allclose(
                inA.value * inB.value,
                outA.value
            ),
            True
        )

    def test_node_C_RTTR_callback_2(self):
        """Test chinet RTTR C callbacks"""
        node = cn.Node(
            {
                "inA": cn.Port(
                    value=[2., 3., 4.],
                    fixed=False,
                    is_output=False
                ),
                "inB": cn.Port(
                    value=[2., 3., 4.],
                    fixed=False,
                    is_output=False
                ),
                "outA": cn.Port(
                    value=0,
                    fixed=False,
                    is_output=True
                )
            }
        )
        node.set_callback("multiply_double", "C")
        self.assertEqual(node.valid, False)

        node.evaluate()
        self.assertEqual(node.valid, True)

        outA = node.ports["outA"]
        inA = node.ports["inA"]
        inB = node.ports["inB"]

        self.assertEqual(
            np.allclose(
                inA.value * inB.value,
                outA.value
            ),
            True
        )

    def test_node_python_callback_1(self):
        """Test chinet Node class python callbacks"""
        node = cn.Node()
        portIn1 = cn.Port(55)
        node.add_input_port("portA", portIn1)
        portIn2 = cn.Port(2)
        node.add_input_port("portB", portIn2)
        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        cb = NodeCallbackMultiply()
        cb.thisown = 0
        node.set_callback(cb)
        node.evaluate()

        self.assertEqual(
            portOut1.value,
            portIn1.value * portIn2.value
        )

    def test_node_python_callback_2(self):
        """Test chinet Node class python callbacks"""
        node = cn.Node()
        portIn1 = cn.Port(55)
        node.add_input_port("portA", portIn1)
        portIn2 = cn.Port(2)
        node.add_input_port("portB", portIn2)
        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)
        node.set_callback(
            NodeCallbackMultiply().__disown__()
        )
        node.evaluate()
        self.assertEqual(
            portOut1.value,
            portIn1.value * portIn2.value
        )

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

    def test_node_valid(self):
        """
        In this test the node node_1 has one inputs (inA) and one output:

                 (inA)-(node_1)-(outA)

        In this example all ports are "non-reactive" meaning, when the input
        of a node changes, the node is set to invalid. A node is set to valid
        when it is evaluated. When a node is initialized it is invalid.
        """
        out_node_1 = cn.Port(
            1.0,
            fixed=False,
            is_reactive=True,
            is_output=True
        )
        in_node_1 = cn.Port(3.0)
        node_1 = cn.Node(
            {
                'inA': in_node_1,
                'outA': out_node_1
            }
        )
        cb = CallbackNodePassOn()
        node_1.set_callback(cb)

        self.assertListEqual(
            list(out_node_1.value),
            [1.0]
        )
        self.assertListEqual(
            list(in_node_1.value),
            [3.0]
        )
        self.assertEqual(node_1.valid, False)
        node_1.evaluate()
        self.assertEqual(node_1.valid, True)

    def test_node_valid_reactive_port(self):
        """
        In this test the node node_1 has one inputs (inA) and one output:

                 (inA)-(node_1)-(outA)

        The input inA is reactive, meaning, when the value of inA changes
        the node is evaluated.
        """

        out_node_1 = cn.Port(
            value=1.0,
            fixed=False,
            is_output=True
        )
        in_node_1 = cn.Port(
            value=3.0,
            fixed=False,
            is_output=False,
            is_reactive=True
        )
        node_1 = cn.Node(
            {
                'inA': in_node_1,
                'outA': out_node_1
            }
        )
        cb = CallbackNodePassOn()
        node_1.set_callback(cb)

        self.assertEqual(node_1.valid, False)

        # A reactive port calls Node::evaluate when its value changes
        in_node_1.value = 12
        self.assertEqual(node_1.valid, True)

        self.assertEqual(
            list(out_node_1.value),
            [12]
        )

    def test_node_valid_connected_nodes(self):
        """
        In this test the two nodes node_1 and node_2 are connected.

        node_1 has one inputs (inA) and one output:

                 (inA)-(node_1)-(outA)

        node_2 has one input (inA) and one output. The input of node_2 is
        connected to the output of node_1:

                (inA->(Node1, outA))-(node_2)-(outA)

        In this example all ports are "non-reactive" meaning, when the input
        of a node changes, the node is set to invalid. A node is set to valid
        when it is evaluated. When a node is initialized it is invalid.
        """
        in_node_1 = cn.Port(
            value=3.0,
            fixed=False,
            is_output=False,
            is_reactive=False
        )
        out_node_1 = cn.Port(
            value=1.0,
            fixed=False,
            is_output=True
        )
        node_1 = cn.Node(
            {
                'inA': in_node_1,
                'outA': out_node_1
            }
        )
        node_1.set_callback("passthrough", "C")

        self.assertListEqual(
            list(in_node_1.value),
            [3.0]
        )
        self.assertListEqual(
            list(out_node_1.value),
            [1.0]
        )

        self.assertEqual(node_1.valid, False)
        node_1.evaluate()
        self.assertEqual(node_1.valid, True)
        self.assertListEqual(
            list(in_node_1.value),
            list(out_node_1.value)
        )
        in_node_2 = cn.Port(
            value=13.0,
            fixed=False,
            is_output=False,
            is_reactive=False
        )
        out_node_2 = cn.Port(
            value=1.0,
            fixed=False,
            is_output=True
        )
        node_2 = cn.Node(
            {
                'inA': in_node_2,
                'outA': out_node_2
            }
        )
        node_2.set_callback("passthrough", "C")
        in_node_2.link = out_node_1

        self.assertEqual(node_2.valid, False)
        node_2.evaluate()
        self.assertEqual(node_2.valid, True)
        self.assertEqual(
            list(out_node_2.value),
            [3.0]
        )

        in_node_1.value = 13
        self.assertEqual(node_1.valid, False)
        self.assertEqual(node_2.valid, False)

        node_1.evaluate()
        self.assertEqual(
            list(out_node_1.value),
            [13.0]
        )

        node_2.evaluate()
        self.assertEqual(
            list(out_node_2.value),
            [13.0]
        )

    def test_node_valid_connected_nodes_reactive_ports(self):
        """
        In this test the two nodes node_1 and node_2 are connected.

        node_1 has one inputs (inA) and one output:

                 (inA)-(node_1)-(outA)

        node_2 has one input (inA) and one output. The input of node_2 is
        connected to the output of node_1:

                (inA->(node_1, outA))-(node_2)-(outA)

        In this example all ports are "non-reactive" meaning, when the input
        of a node changes, the node is set to invalid. A node is set to valid
        when it is evaluated. When a node is initialized it is invalid.
        """
        in_node_1 = cn.Port(
            value=3.0,
            fixed=False,
            is_output=False,
            is_reactive=True
        )
        out_node_1 = cn.Port(
            value=1.0,
            fixed=False,
            is_output=True
        )
        node_1 = cn.Node(
            {
                'inA': in_node_1,
                'outA': out_node_1
            }
        )
        node_1.set_callback("passthrough", "C")

        in_node_2 = cn.Port(
            value=1.0,
            fixed=False,
            is_output=False,
            is_reactive=True
        )
        out_node_2 = cn.Port(
            value=1.0,
            fixed=False,
            is_output=True
        )
        node_2 = cn.Node(
            {
                'inA': in_node_2,
                'outA': out_node_2
            }
        )
        node_2.set_callback("passthrough", "C")
        in_node_2.link = out_node_1
        in_node_1.value = 13

        self.assertEqual(node_1.valid, True)
        self.assertEqual(node_2.valid, True)
        self.assertEqual(
            list(out_node_2.value),
            [13.0]
        )


if __name__ == '__main__':
    unittest.main()
