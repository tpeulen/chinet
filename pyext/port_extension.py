@property
def bytes(self):
    # type: () -> np.ndarray
    return self.get_bytes()


@bytes.setter
def bytes(self, v):
    # type: () -> (np.ndarray)
    self.set_bytes(v)


@property
def link(self):
    # type: () -> cn.Port
    return self.get_link()


@link.setter
def link(self, v) -> None:
    self.set_link(v)


@property
def value(self):
    value_getters = {
        0: self.get_value_vi,
        1: self.get_value_vd,
        2: lambda: self.get_value_vi()[0],
        3: lambda: self.get_value_vd()[0],
    }
    return value_getters.get(self.get_value_type(), lambda: None)()


@value.setter
def value(self, v):
    if not isinstance(v, np.ndarray):
        v = np.atleast_1d(v)
    if v.dtype.kind == 'i':
        self.set_value_vi(v)
    else:
        self.set_value_vd(v)


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
    self.value = np.atleast_1d(value)
    self.fixed = fixed


def __repr__(self):
    return "Port(%s)" % self.oid


def __str__(self):
    return self.get_json(indent=4)

