
# creating a Node object
########################

################################################################################
# without arguments the Node object connects to
# "mongodb://localhost:27017", the local MongoDB server,
# and creates of new Node entry in the "db_chinet.nodes" collection
# the ports are written/added to the DB
from __future__ import print_function
import chinet as cn

portA = cn.Port(open('./examples/nodes/input.json').read())
portB = cn.Port(open('./examples/nodes/output.json').read())
curve = cn.Port(open('./examples/nodes/curve.json').read())

uri_string = "mongodb://localhost:27017"
oid_string = "5caea5d371323f06b6473262"
node = cn.Node(uri_string, oid_string, portA, portB, "NodeCallback2", "C")

################################################################################
# reading the ports from the DB
from __future__ import print_function
import chinet as cn

uri_string = "mongodb://localhost:27017"
oid_string = "5caea5d371323f06b6473262"
portA_oid = "5caea5d371323f06b6473262"
portB_oid = "5caea5d371323f06b6473214"
node = cn.Node(uri_string, oid_string, portA_oid, portB_oid, "NodeCallback2", "C")


################################################################################
from __future__ import print_function
import chinet as cn

uri_string = "mongodb://localhost:27017"
portA = cn.Port(open('./examples/nodes/input.json').read())
portB = cn.Port(open('./examples/nodes/output.json').read())

class NodeCallback(cn.NodeCallback):

    def __init__(self, *args, **kwargs):
        cn.NodeCallback.__init__(self, *args, **kwargs)

    def run(self, input, output):
        print("This print from Python")

cb = NodeCallback('operation')
node = cn.Node(uri_string, portA, portB, cb)


# passing a uri string upon creation the MongoDB server can be specified
node = cn.Node(uri_string)

# ports
portA = cn.Port(open('./examples/nodes/input.json').read())
portB = cn.Port(open('./examples/nodes/output.json').read())
node.set_input_port(portA)
node.set_output_port(portB)
node.to_json()
node.write_to_db()


# From nodes and port in database
node = cn.Node(
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

class NodeCallback(cn.NodeCallback):

    def __init__(self, *args, **kwargs):
        cn.NodeCallback.__init__(self, *args, **kwargs)

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
node = cn.Node(uri_string, portA, portB, NodeCallback('operation'))
print(node.get_oid())

# Alternatively, the uri string is not provided -> local MongoDB server
node = cn.Node(portA, portB, cb)
print(node.get_oid())


# registered methods
node = cn.Node(
    uri_string,
    portA.get_oid(),
    portB.get_oid(),
    "NodeCallback2",
    "C"
)
node.update()

# registered methods
import chinet as cn
uri_string = "mongodb://localhost:27017"
node = cn.Node(
    uri_string,
    open('./examples/nodes/node.json').read()
)
port_input = node.get_input_port()
port_output = node.get_output_port()
print(port_input.get_slot_value("slotA1"))
port_input.set_slot_value("slotA1", 888)

node2 = cn.Node(
    uri_string,
    open('./examples/nodes/node.json').read()
)
port_input = node2.get_input_port()
print(node2.get_input_port().get_slot_value("slotA1"))
print(port_input.get_slot_value("slotA1"))
