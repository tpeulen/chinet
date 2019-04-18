from __future__ import print_function
import fluomodlib as flm

# creating a Node object
########################

# without arguments the Node object connects to "mongodb://localhost:27017", the local MongoDB server,
# and creates of new Node entry in the "db_mofa.nodes" collection
node = flm.Node()
print(node.get_oid())

uri_string = "mongodb://localhost:27017"
# passing a uri string upon creation the MongoDB server can be specified
node = flm.Node(uri_string)
portA = flm.Port(open('./examples/nodes/portA.json').read())
portB = flm.Port(open('./examples/nodes/portB.json').read())
node.set_input_port(portA)
node.set_output_port(portB)
node.write_to_db()


# From nodes and port in database
node = flm.Node(
    uri_string,
    portA.get_oid(),
    portB.get_oid()
)
print(node.get_input_port().get_slot_keys())
print(node.get_output_port().get_slot_keys())


# Callbacks
#################

# Nodes evaluate callbacks, i.e., functions with an input and an output
# callbacks derive from the NodeCallback class.

class NodeCallback(flm.NodeCallback):

    def __init__(self, *args, **kwargs):
        flm.NodeCallback.__init__(self, *args, **kwargs)

    def run(self, input, output):
        print("This print from Python")


cb = NodeCallback('operation')

# Setting the callback for the node evaluation
node.set_callback(cb)
print(node.get_name())
node.update()

# monkey patching callbacks
cb.run = lambda input, output: print("alternative")
cb.name = "op2"
print(node.get_name())
node.update()

# upon creation the uri, the input and output port and the call back can be specified
node = flm.Node(uri_string, portA, portB, NodeCallback('operation'))
print(node.get_oid())

# Alternatively, the uri string is not provided -> local MongoDB server
node = flm.Node(portA, portB, cb)
print(node.get_oid())


# registered methods
node = flm.Node(
    uri_string,
    portA.get_oid(),
    portB.get_oid(),
    "NodeCallback2",
    "C"
)
node.update()


# registered methods
import fluomodlib as flm
uri_string = "mongodb://localhost:27017"
node = flm.Node(
    uri_string,
    open('./examples/nodes/node.json').read()
)
port_input = node.get_input_port()
port_output = node.get_output_port()
print(port_input.get_slot_value("slotA1"))
port_input.set_slot_value("slotA1", 888)

node2 = flm.Node(
    uri_string,
    open('./examples/nodes/node.json').read()
)
port_input = node2.get_input_port()
print(node2.get_input_port().get_slot_value("slotA1"))

print(port_input.get_slot_value("slotA1"))
