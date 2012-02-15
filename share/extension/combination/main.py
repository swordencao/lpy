from openalea.lpy import *
from openalea.lpy.composition import *


def simulation():
  a = Lsystem('A.lpy')
  b = Lsystem('B.lpy')
  c = Lsystem('interpretation.lpy')
  a2b = Lsystem('A2B.lpy')
  b2a = Lsystem('B2A.lpy')  
  a2c = Lsystem('A2C.lpy')
  
  lsystem = ComposedLsystem([a,a2b,b,b2a],[a2c,c])
  lstring = lsystem.axiom
  print lstring
  lsystem.animate(nbsteps = 10)


if __name__ == '__main__':
    simulation()