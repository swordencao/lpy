from openalea.plantgl.gui.curve2deditor import Curve2DEditor,Curve2DConstraint
from OpenGL.GL import *
class Curve2DManager(AbstractPglObjectManager):
    """see the doc of the objectmanager abtsract class to undesrtand the implementation of the functions"""
    def __init__(self):
        AbstractPglObjectManager.__init__(self,"Curve2D")
        
    #def displayThumbnail(self,obj,i,focus,objectthumbwidth):
    def createDefaultObject(self):
        return Curve2DConstraint.defaultCurve()
