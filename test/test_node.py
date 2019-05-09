import utils
import os
import unittest
import sys
import json

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_node_C_RTTR_callback(self):
        """Test chinet RTTR C callbacks"""
        node = cn.Node()

        portIn1 = cn.Port(55)
        node.add_input_port("portA", portIn1)

        portIn2 = cn.Port(2)
        node.add_input_port("portB", portIn2)

        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        node.set_callback("multiply", "C")

        node.update()

        self.assertEqual(portOut1.get_value(), portIn1.get_value() * portIn2.get_value())

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
        node.update()
        self.assertEqual(portOut1.get_value(), portIn1.get_value() * portIn2.get_value())

    def test_node_write_to_db(self):
        uri_string = "mongodb://localhost:27017"
        db_string = "chinet"
        app_string = "chisurf"
        collection_string = "test_collection"

        node = cn.Node()
        node.connect_to_db(
            uri_string=uri_string,
            db_string=db_string,
            app_string=app_string,
            collection_string=collection_string
        )

        portIn1 = cn.Port(55)
        node.add_input_port("portA", portIn1)

        portIn2 = cn.Port(2)
        node.add_input_port("portB", portIn2)

        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        node.set_callback("multiply", "C")
        self.assertEqual(node.write_to_db(), True)

    def test_node_restore_from_db(self):
        uri_string = "mongodb://localhost:27017"
        db_string = "chinet"
        app_string = "chisurf"
        collection_string = "test_collection"

        node = cn.Node()
        node.connect_to_db(
            uri_string=uri_string,
            db_string=db_string,
            app_string=app_string,
            collection_string=collection_string
        )

        portIn1 = cn.Port(55)
        node.add_input_port("portA", portIn1)

        portIn2 = cn.Port(2)
        node.add_input_port("portB", portIn2)

        portOut1 = cn.Port()
        node.add_output_port("portC", portOut1)

        node.set_callback("multiply", "C")
        node.write_to_db()

        node_restore = cn.Node()
        node_restore.connect_to_db(
            uri_string=uri_string,
            db_string=db_string,
            app_string=app_string,
            collection_string=collection_string
        )
        node_restore.read_from_db(node.get_oid_string())

        dict_restore = json.loads(node_restore.get_json())
        dict_original = json.loads(node.get_json())

        self.assertEqual(dict_restore, dict_original)


if __name__ == '__main__':
    unittest.main()