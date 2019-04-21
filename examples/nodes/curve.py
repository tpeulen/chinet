import chinet as cn
import numpy as np

curve = cn.Curve()
x = np.linspace(0, 6, 100)
y = np.sin(x)
curve.set_x(x)
curve.set_y(y)

curve.save('curve.json')
