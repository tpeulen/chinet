import fluomodlib as flm
import numpy as np
import pylab as p


v1 = flm.Node(11, "a")
v1.get_value()

v2 = flm.Node(2, "b")
v2.get_value()

v3 = flm.Node()
v3.set_input_ports(flm.VectorNode([v1, v2]))
v3.set_operation("multiply", flm.get_node_operation_multiply())

v4 = v1 * v2
print v4.get_value()

v4.get_name()


# Nodes without a name have a name pointing to the memory address
v6 = flm.Node(11)
