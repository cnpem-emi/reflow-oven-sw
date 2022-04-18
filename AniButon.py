import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from random import randint
import matplotlib.widgets

class AniButon(FuncAnimation):
    def __init__(self, fig, func, stop, pos=(0.125, 0.92), **kwargs):
        self.i = 0
        self.fig = fig
        self.func = func
        self.stop = stop
        self.setup(pos)
        FuncAnimation.__init__(self,self.fig, self.func, **kwargs )    
    def setup(self, pos):
        playerax = self.fig.add_axes([pos[0],pos[1], 0.22, 0.04])
        self.button_stop = matplotlib.widgets.Button(playerax, label=u'$\u25A0$')
        self.button_stop.on_clicked(self.stop)
fig, ax = plt.subplots()
x = np.linspace(0,6*np.pi, num=100)
y = np.sin(x)
ax.plot(x,y)
point, = ax.plot([],[], marker="o", color="crimson", ms=15)
#def update(i):

