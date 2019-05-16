import utils
import os
import unittest
import sys
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


if __name__ == '__main__':
    unittest.main()