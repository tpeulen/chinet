import fluomodlib as flm
import numpy as np

curve = flm.Curve()
x = np.linspace(0, 6, 100)
y = np.sin(x)
curve.set_x(x)
curve.set_y(y)

curve.save('curve.json')
