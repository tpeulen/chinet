
def __str__(self):
    return self.get_json(indent=4)


def __repr__(self):
    s = "MongoObject(%s)" % self.oid
    return s


def __getitem__(self, key):
    return self[key]
