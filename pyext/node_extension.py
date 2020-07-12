@property
def inputs(self):
    return self.get_input_ports()


@property
def outputs(self):
    return self.get_output_ports()


@property
def ports(self):
    return self.get_ports()


def callback_function(
        self,
        cb: typing.Callable
):
    class CallbackNodePython(cn.NodeCallback):

        def __init__(
                self,
                cb_function: typing.Callable,
                *args, **kwargs
        ):
            super().__init__(*args, **kwargs)
            self._cb = cb_function

        def run(self, inputs, outputs):
            input_dict = dict(
                [
                    (inputs[i].name, inputs[i].value) for i in inputs
                ]
            )
            output = self._cb(**input_dict)
            if isinstance(output, dict):
                for key, ov in zip(outputs, output):
                    outputs[key].value = output[key]
            elif isinstance(output, tuple):
                for key, ov in zip(outputs, output):
                    outputs[key].value = ov
            else:
                next(iter(outputs.values())).value = output
    self.set_string('source', inspect.getsource(cb))
    # When the cb-function of a node is evaluated, the input Ports are
    # processed by the callback and written to the output Port. Thus,
    # the inputs and outputs need to be defined as Ports. Here, the inputs
    # and the output of the function are inspected to create the Ports of the
    # None.
    # get the signature of the function to create input Ports
    signature = inspect.signature(cb)
    input_ports = list()
    for parameter_name in signature.parameters:
        o = signature.parameters[parameter_name]
        if o.default is inspect.Signature.empty:
            if o.annotation is int:
                value = 1
            elif o.annotation is np.ndarray:
                value = np.array([1.0], dtype=np.double)
            else:
                value = 1.0
        else:
            value = o.default
        p = cn.Port(
            name=o.name,
            value=value,
            is_output=False
        )
        input_ports.append(p)
        self.add_input_port(
            key=o.name,
            port=p
        )
    # run the function once with the default values to get the output
    input_values = dict([(p.name, p.value) for p in input_ports])
    returned_value = cb(**input_values)
    # use the return value of the function to create output Ports
    output_values = list()
    output_names = list()
    if isinstance(returned_value, dict):
        for ok in returned_value:
            output_values.append(returned_value[ok])
            output_names.append(ok)
    elif isinstance(returned_value, tuple):
        output_values = returned_value
        for i in range(len(returned_value)):
            output_names.append("out_" + str(i).zfill(2))
    else:
        output_names = 'out_00',
        output_values = returned_value,
    for on, ov in zip(output_names, output_values):
        self.add_output_port(
            key=on,
            port=cn.Port(
                name=on,
                value=ov,
                is_output=True
            )
        )
    # create a new CallbackNodePython
    cb_instance = CallbackNodePython(cb_function=cb)
    cb_instance.__disown__()
    self.set_callback(cb_instance)


callback_function = property(None, callback_function)


def __init__(
        self,
        name="",
        ports=None,
        callback_function=None,
        *args, **kwargs
):
    this = _chinet.new_Node(*args, **kwargs)
    try:
        self.this.append(this)
    except:
        self.this = this
    self.register_instance(None)
    if callable(callback_function):
        self.callback_function = callback_function
    elif isinstance(ports, dict):
        self.set_ports(ports)
    if len(name) > 0:
        self.name = name

def __del__(self):
    super().__del__()
