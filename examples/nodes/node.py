from __future__ import print_function
import fluomodlib as flm

portA = flm.Port(open('./examples/nodes/portA.json').read())
portB = flm.Port(open('./examples/nodes/portB.json').read())

node = flm.Node()
node.set_input_port(portA)
node.set_output_port(portB)


class NodeCallback(flm.NodeCallback):

    def run(self, input, output):
        print("This print from Python")
        print(input.get_name())
        print(output.get_name())


cb = NodeCallback('op1')
node.set_callback(cb)
print(node.get_name())
node.update()


cb.run = lambda input, output: print("alternative")
cb.name = "op2"
print(node.get_name())
node.update()


node.set_callback(NodeCallback())
print(node.get_name())
node.update()


node2 = flm.Node(portA, portB, cb)

#node = flm.Node("A", input=portA, output=portB, callback=call)
