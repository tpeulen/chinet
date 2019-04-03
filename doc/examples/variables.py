import fluomodlib as flm
import numpy as np
import pylab as p


v1 = flm.Variable(11, "a")
v1.get_value()

v2 = flm.Variable(2, "b")
v2.get_value()

v3 = v1 * v2
v3.get_name()


v3 = flm.Variable()
v3.link(flm.vector_Variable([v1,v2]), flm.get_mul())

