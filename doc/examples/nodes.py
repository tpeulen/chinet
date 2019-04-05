import fluomodlib as flm

n1 = flm.Node("a")
n1_pa = flm.Port("a1")
n1_pa.get_value()

json_string = open('curve.json').read()

n1_pa.read_json(json_string)
n1_pa.get_json()


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
