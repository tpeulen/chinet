from __future__ import print_function
import chinet as cn


p = cn.Port()
v = cn.Value([717, 727], False)
p.set_v("sa", v)

p2 = cn.Port()
p2.set_v('sq', cn.Value(), cn.Link(p, 'sa'))
print(p2.get_v('sq').to_json())
