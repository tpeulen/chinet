from __future__ import print_function
import fluomodlib as flm

# creating a Node object
########################

# without arguments the Node object connects to the local MongoDB server
node = flm.Node()
print(node.get_oid())

# passing a uri string upon creation the MongoDB server can be specified
uri_string = "mongodb://localhost:27017"
node = flm.Node(uri_string)
print(node.get_oid())

# assignment of ports - this adds the ports to the DB
portA = flm.Port(open('./examples/nodes/portA.json').read())
portB = flm.Port(open('./examples/nodes/portB.json').read())
node.set_input_port(portA)
node.set_output_port(portB)

# From nodes and port in database
node = flm.Node(
    uri_string,
    portA.get_oid(),
    portB.get_oid()
)
print(node.get_input_port().get_slot_keys())
print(node.get_output_port().get_slot_keys())


# From nodes and port in database
node = flm.Node(
    uri_string,
    portA.get_oid(),
    portB.get_oid(),
    "flm.",
    "C"
)
print(node.get_input_port().get_slot_keys())
print(node.get_output_port().get_slot_keys())


# Callbacks
#################

# Nodes evaluate callbacks, i.e., functions with an input and an output
# callbacks derive from the NodeCallback class.

class NodeCallback(flm.NodeCallback):

    def run(self, input, output):
        print("This print from Python")


cb = NodeCallback('operation')

# Setting the callback for the node evaluation
node.set_callback(cb)
print(node.get_name())
node.update()

# monkey patching callbacks
cb.run = lambda self, input, output: print("alternative")
cb.name = "op2"
print(node.get_name())
node.update()

# upon creation the uri, the input and output port and the call back can be specified
node = flm.Node(uri_string, portA, portB, cb)
print(node.get_oid())

# Alternatively, the uri string is not provided -> local MongoDB server
node = flm.Node(portA, portB, cb)
print(node.get_oid())




#node = flm.Node("A", input=portA, output=portB, callback=call)

