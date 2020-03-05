# The content of this file is embedded by SWIG into the Port class. The code
# below extends the Port class.

# The approach used below with __swig_getmethods is deprecated in swig 4.0.0
# Thus, the code below needs to be changed in the future.
__swig_getmethods__["bytes"] = get_bytes
__swig_setmethods__["bytes"] = set_bytes
if _newclass: bytes = property(get_bytes, set_bytes)

__swig_getmethods__["link"] = get_link
__swig_setmethods__["link"] = set_link
if _newclass: link = property(get_link, set_link)


@property
def value(self):
    if self.value_type == 0:
        return self.get_value_i()
    else:
        return self.get_value_d()


@value.setter
def value(self, v):
    if not isinstance(v, np.ndarray):
        v = np.atleast_1d(v)
    if v.dtype.kind == 'i':
        self.set_value_i(v, True)
    else:
        self.set_value_d(v, True)


@property
def bounds(self):
    if self.bounded:
        return self.get_bounds()
    else:
        return None, None


@bounds.setter
def bounds(self, v):
    self.set_bounds(np.array(v, dtype=np.float64))


def __init__(
        self,
        value=[],
        fixed=False,
        *args, **kwargs
):
    this = _chinet.new_Port(*args, **kwargs)
    try:
        self.this.append(this)
    except:
        self.this = this
    if not isinstance(value, np.ndarray):
        value = np.atleast_1d(value)
    if value.dtype.kind == 'i':
        self.set_value_i(value)
    else:
        self.set_value_d(value)
    self.fixed = fixed


def __repr__(self):
    return "Port(%s)" % self.oid


def __str__(self):
    return self.get_json(indent=4)

