from __future__ import print_function
import fluomodlib as flm


# Creating a port and reading content from JSON string
portA = flm.Port()
json_string = open('./examples/nodes/portA.json').read()
portA.from_json(json_string)
print(portA.get_slot_value("NameA1"))
print(portA.get_slot_keys())
print(portA.get_slot_values())

# Creating a port from JSON string
portB = flm.Port(open('./examples/nodes/portB.json').read())

# Assigning an input redirects the content
portA.set_input(portB)
print(portA.get_slot_values())
print(portA.get_slot_keys())



'''
print json_string
print input.to_json()

input.set_slot_value("aa", 22)


# By default a Node is a pass through (output = input)
input.to_json() == node_1.get_output_port().to_json()


#################################

n1_po = flm.Port("outA1", "portA.json")
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
'''