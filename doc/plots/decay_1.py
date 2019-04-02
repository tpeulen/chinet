import fluomodlib as flm
import numpy as np
import pylab as p

x, y = np.loadtxt("./examples/tcspc/ibh/Prompt.txt", skiprows=9).T
irf = flm.Curve()
irf.set_x(x)
irf.set_y(y)

dt = 0.0141
nx = 4096
d = flm.Decay(dt, nx)
d.set_instrument_response_function(irf)
d.append(0.1, 4)
d.append(2, 1)
d.update()

p.plot(
    d.get_x(),
    d.get_y()
)
p.show()

