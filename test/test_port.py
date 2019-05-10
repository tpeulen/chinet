import utils
import os
import unittest
import sys
import json

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
        self.assertEqual(p1.get_value(), p2.get_value())

        # check fixing
        p3 = cn.Port(v1, True)
        self.assertEqual(p3.is_fixed(), True)
        p4 = cn.Port(v1, False)
        self.assertEqual(p4.is_fixed(), False)

        # check linking
        p5 = cn.Port(v2, False, p4)
        self.assertEqual(p5.get_value(), p4.get_value())

    def test_port_init_vector(self):
        """Test chinet Port class set_value and get_value"""
        v1 = [1, 2, 3, 5, 8]
        v2 = [1, 2, 4, 8, 16]
        p1 = cn.Port()
        p1.set_array(v1)

        # check setting of value
        p2 = cn.Port(v1)
        self.assertEqual(p1.get_array(), p2.get_array())

        # check fixing
        p3 = cn.Port(v1, True)
        self.assertEqual(p3.is_fixed(), True)
        p4 = cn.Port(v1, False)
        self.assertEqual(p4.is_fixed(), False)

        # check linking
        p5 = cn.Port(v2, False, p4)
        self.assertEqual(p5.get_array(), p4.get_array())

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

    def test_db_write(self):
        uri_string = "mongodb://localhost:27017"
        db_string = "chinet"
        app_string = "chisurf"
        collection_string = "test_collection"

        value_array = (1, 2, 3, 5, 8, 13)
        value = 17

        port = cn.Port(value)
        port.set_array(value_array)
        port.set_fixed(True)

        port.connect_to_db(
            uri_string=uri_string,
            db_string=db_string,
            app_string=app_string,
            collection_string=collection_string
        )
        print(port.get_json())

        self.assertEqual(port.write_to_db(), True)

    def test_port_db_restore(self):
        uri_string = "mongodb://localhost:27017"
        db_string = "chinet"
        app_string = "chisurf"
        collection_string = "test_collection"

        value_array = (1, 2, 3, 5, 8, 13)
        value = 17

        port = cn.Port()
        port.set_value(value)
        port.set_array(value_array)

        port.connect_to_db(
            uri_string=uri_string,
            db_string=db_string,
            app_string=app_string,
            collection_string=collection_string
        )
        port.write_to_db()

        port_reload = cn.Port()
        port_reload.connect_to_db(
            uri_string=uri_string,
            db_string=db_string,
            app_string=app_string,
            collection_string=collection_string
        )
        self.assertEqual(port_reload.read_from_db(port.get_oid()), True)

        dict_port = json.loads(port.get_json())
        dict_port_restore = json.loads(port.get_json())

        self.assertEqual(dict_port, dict_port_restore)


if __name__ == '__main__':
    unittest.main()