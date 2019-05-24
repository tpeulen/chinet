import utils
import os
import unittest
import json
import numpy as np

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_port_init_singelton(self):
        """Test chinet Port class set_value and get_value"""
        v1 = 23.0
        v2 = 29.0
        p1 = cn.Port()
        p1.set_value(v1)

        # check setting of value
        p2 = cn.Port(v1)

        # check fixing
        p3 = cn.Port(v1, True)
        p4 = cn.Port(v1, False)

        self.assertEqual(p1.get_value(), p2.get_value())
        self.assertEqual(p3.is_fixed(), True)
        self.assertEqual(p4.is_fixed(), False)

        # check linking
        p5 = cn.Port(v2)
        p5.link(p4)
        self.assertEqual(p5.get_value(), p4.get_value())

    def test_port_init_vector(self):
        """Test chinet Port class set_value and get_value"""
        v1 = [1, 2, 3, 5, 8]
        v2 = [1, 2, 4, 8, 16]
        p1 = cn.Port()
        p1.set_value(v1)

        # check setting of value
        p2 = cn.Port(v1)
        self.assertListEqual(list(p2.get_value()), list(p1.get_value()))

        # check fixing
        p3 = cn.Port(v1, True)
        self.assertEqual(p3.is_fixed(), True)
        p4 = cn.Port(v1, False)
        self.assertEqual(p4.is_fixed(), False)

        # check linking
        p5 = cn.Port(v2, False)
        p5.link(p4)
        self.assertListEqual(list(p5.get_value()), list(p4.get_value()))

    def test_port_init_array(self):
        """Test chinet Port class set_value and get_value"""
        array = np.array([1, 2, 3, 5, 8, 13], dtype=np.double)
        p1 = cn.Port()
        p1.set_value(array)
        p2 = cn.Port(array)
        self.assertListEqual(list(p1.get_value()), list(p2.get_value()))

    def test_set_get_value_1(self):
        """Test chinet Port class set_value and get_value"""
        value = 23.0
        port = cn.Port(value)
        self.assertEqual(port.get_value(), value)

    def test_set_get_value_2(self):
        """Test chinet Port class set_value and get_value"""
        value = (1,)
        port = cn.Port()
        port.set_value(value)
        self.assertEqual(port.get_value(), value)

    def test_port_link_value(self):
        value1 = np.array([12], dtype=np.double)
        value2 = np.array([6], dtype=np.double)
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

    def test_db_write(self):
        db_dict = {
            'uri_string': "mongodb://localhost:27017",
            'db_string': "chinet",
            'app_string': "chisurf",
            'collection_string': "test_collection"
        }

        value_array = (1, 2, 3, 5, 8, 13)
        value = 17

        port = cn.Port(value)
        port.set_value(value_array)
        port.set_fixed(True)

        port.connect_to_db(**db_dict)
        print(port.get_json())

        self.assertEqual(port.write_to_db(), True)

    def test_port_db_restore(self):
        db_dict = {
            'uri_string': "mongodb://localhost:27017",
            'db_string': "chinet",
            'app_string': "chisurf",
            'collection_string': "test_collection"
        }

        value_array = (1, 2, 3, 5, 8, 13)
        value = 17

        port = cn.Port()
        port.set_value(value)
        port.set_value(value_array)

        port.connect_to_db(**db_dict)
        port.write_to_db()

        port_reload = cn.Port()
        port_reload.connect_to_db(**db_dict)
        self.assertEqual(port_reload.read_from_db(port.get_oid()), True)

        dict_port = json.loads(port.get_json())
        dict_port_restore = json.loads(port.get_json())

        self.assertEqual(dict_port, dict_port_restore)


if __name__ == '__main__':
    unittest.main()