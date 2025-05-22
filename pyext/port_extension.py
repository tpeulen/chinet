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
def link(self, v):
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

    # Handle NaN, infinity, and other invalid values
    if v.dtype.kind in ['f', 'd']:
        # Replace NaN with a very small number
        v = np.where(np.isnan(v), np.finfo(np.float64).tiny, v)
        # Replace infinity with very large/small numbers
        v = np.where(np.isinf(v) & (v > 0), np.finfo(np.float64).max, v)
        v = np.where(np.isinf(v) & (v < 0), np.finfo(np.float64).min, v)

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
    # Convert to numpy array
    v_array = np.array(v, dtype=np.float64)

    # Handle NaN, infinity, and other invalid values
    if v_array.size == 1:
        # If a single value is provided and it's NaN, use default bounds
        if np.isnan(v_array[0]):
            v_array = np.array([np.finfo(np.float64).min, np.finfo(np.float64).max], dtype=np.float64)
    elif v_array.size >= 2:
        # For arrays with at least 2 elements, replace NaN values individually
        if np.isnan(v_array[0]):
            v_array[0] = np.finfo(np.float64).min
        if np.isnan(v_array[1]):
            v_array[1] = np.finfo(np.float64).max

        # Ensure lower bound is not -inf and upper bound is not +inf
        if np.isinf(v_array[0]) and v_array[0] < 0:
            v_array[0] = np.finfo(np.float64).min
        if np.isinf(v_array[1]) and v_array[1] > 0:
            v_array[1] = np.finfo(np.float64).max

    self.set_bounds(v_array)

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

    # Use the value setter which already handles NaN and infinity
    self.value = np.atleast_1d(value)
    self.fixed = fixed

def __repr__(self):
    return "Port(%s)" % self.oid

def __str__(self):
    return self.get_json(indent=4)
