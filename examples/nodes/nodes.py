import fluomodlib as flm

node_1 = flm.Node("A")
n1_pi = flm.Port("inA1")
node_1.set_input_port(n1_pi)

json_string = open('./examples/nodes/curve.json').read()
n1_pi.read_json(json_string)
# By default a Node is a pass through (output = input)
n1_pi.get_json() == node_1.get_output_port().get_json()


#################################

n1_po = flm.Port("outA1", "port.json")
node_1.set_output_port(n1_po)



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
