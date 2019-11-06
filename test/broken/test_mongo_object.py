import utils
import os
import unittest
import json

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
utils.set_search_paths(TOPDIR)

import chinet as cn


class Tests(unittest.TestCase):

    def test_mongo_init(self):
        mo_name = "test_name"
        mo = cn.MongoObject()
        mo.set_name(mo_name)
        self.assertEqual(mo.get_name(), cn.MongoObject(mo_name).get_name())

    def test_mongo_db_connect(self):
        db_dict = {
            'uri_string': "mongodb://localhost:27017",
            'db_string': "chinet",
            'app_string': "chisurf",
            'collection_string': "test_collection"
        }
        mo = cn.MongoObject()
        self.assertEqual(mo.connect_to_db(**db_dict), True)
        self.assertEqual(mo.is_connected_to_db(), True)
        mo.write_to_db()
        mo.disconnect_from_db()
        self.assertEqual(mo.is_connected_to_db(), False)

        mo2 = cn.MongoObject()
        mo.connect_object_to_db_mongo(mo2)
        self.assertEqual(mo2.is_connected_to_db(), True)
        mo2.disconnect_from_db()
        self.assertEqual(mo2.is_connected_to_db(), False)

        # mo3 = cn.MongoObject()
        # mo3.connect_to_db(
        #     **db_dict
        # )
        # mo3.read_from_db(mo.get_oid())
        # self.assertEqual(mo3.read_from_db(mo.get_oid()), True)

    def test_mongo_oid(self):
        mo = cn.MongoObject()
        self.assertEqual(len(mo.get_oid()), 25)

    def test_mongo_json(self):
        mo = cn.MongoObject("test_name")
        d = json.loads(mo.get_json())
        self.assertEqual(set(d.keys()), set(['_id', 'precursor', 'death', 'name']))

    def test_singleton(self):
        mo = cn.MongoObject()
        mo.set_singleton_double("d", 22.3)
        self.assertAlmostEqual(mo.get_singleton_double("d"), 22.3)

        mo.set_singleton_int("i", 13)
        self.assertEqual(mo.get_singleton_int("i"), 13)

        mo.set_singleton_bool("b1", True)
        self.assertEqual(mo.get_singleton_bool("b1"), True)

        mo.set_singleton_bool("b2", False)
        self.assertEqual(mo.get_singleton_bool("b2"), False)

    def test_array(self):
        mo = cn.MongoObject()
        mo.set_array_double("d", (1.1, 2.2))
        self.assertTupleEqual(mo.get_array_double("d"), (1.1, 2.2))

        mo.set_array_int("i", [3, 4])
        self.assertEqual(mo.get_array_int("i"), (3, 4))

    def test_read_json(self):
        json_file = "./inputs/session_template.json"

        json_string = ""
        with open(json_file, 'r') as fp:
            json_string = fp.read()

        mo = cn.MongoObject()
        mo.read_json(json_string)
        sub_json = mo.get_json("nodes")

        mo2 = cn.MongoObject()
        mo2.read_json(sub_json)

        subset = json.loads(json_string)
        superset = json.loads(mo2.get_json())

        self.assertEqual(
            all(item in superset.items() for item in subset.items()),
            True
        )

        subset = json.loads(json_string)
        superset = json.loads(mo["nodes"].get_json())
        self.assertEqual(
            all(item in superset.items() for item in subset.items()),
            True
        )



if __name__ == '__main__':
    unittest.main()