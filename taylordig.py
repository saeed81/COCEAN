import numpy as np
import matplotlib.pyplot as plt
import math

#matplotlib.rcParams['xtick.direction'] = 'out'
#matplotlib.rcParams['ytick.direction'] = 'out'
#plt.tick_params(axis="y", labelcolor="b")

X=np.arange(0.0,1.1,0.01)
Y=np.arange(0.0,1.1,0.01)

print X.shape 

print Y.shape

nx=X.shape[0]
ny=Y.shape[0]

h = np.zeros((ny,nx),dtype=np.float32)

for j in range(ny):
    for i in range(nx):
        h[j,i] = math.sqrt(X[i] * X[i] + Y[j] * Y[j])

fig=plt.figure(num=1,figsize=(3.0,3.0),dpi=300,facecolor='w',edgecolor='k')
ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
ax.set_xlabel('RMS/OBS_STD',fontsize='5',weight='bold')
ax.set_ylabel ('RMS/OBS_STD',fontsize='5',weight='bold')
ax.grid(False)
vc=np.arange(0.0,1.2,0.2)

ax.contour(X,Y,h,vc,colors='0.5',linewidths=0.2)
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.set_xlim(0.0,1.005)
ax.set_ylim(0.0,1.005)
ax.yaxis.set_ticks_position('left')
ax.xaxis.set_ticks_position('bottom')

radius = np.arange(0.0,1.20,0.20)
xangle = [1.0,0.99,0.95] + list(np.arange(0.9,-0.1,-0.1))
xangle = list(np.arange(0.0,1.10,0.10))

#print xangle

for ang in xangle:
    print ang, math.degrees(math.acos(ang))
nline = 0
for rd in radius:  
    for ang in xangle:
        #print math.cos(math.acos(ang))
        if ang == 0.99 or ang == 0.95:
            ax.plot([0.0,rd*math.cos(math.acos(ang))],[0.0,rd*math.sin(math.acos(ang))],'k-',lw="0.0")
        else:
            #ax.plot([0.0,rd*math.cos(math.acos(ang))],[0.0,rd*math.sin(math.acos(ang))],'k-',lw="0.25")
            #ax.plot([0.0,0.0],[rd*math.cos(math.acos(ang)),rd*math.sin(math.acos(ang))],'k-',lw="0.25")
            #ax.plot([0.0,1.0],[0.0,1.0],'k-',lw="0.25")
            if nline == 0:
                nline += 1
                ax.plot([0.0,rd*ang],[0.0,rd*math.sqrt(1.0 - (ang * ang))],color="0.5",ls="-",label="viken",lw="0.2")
                if rd == 1.0: ax.annotate(str(ang),  xy=(rd*ang, rd*math.sqrt(1.0 - (ang * ang))))
            else:
                nline += 1
                ax.plot([0.0,rd*ang],[0.0,rd*math.sqrt(1.0 - (ang * ang))],color="0.5",ls="-",lw="0.2")
                if rd == 1.0 and ang > 0.0: ax.annotate(str(ang),  xy=(rd*ang, rd*math.sqrt(1.0 - (ang * ang))))

ax.plot(0.2*math.cos(math.acos(0.5)),0.2*math.sin(math.acos(0.5)),'ob',ms=3)

#ax.legend(numpoints=1,prop=dict(size='xx-small'),loc = 'upper right', bbox_to_anchor = (0.5, 0.5))
ax.legend(numpoints=1,prop=dict(size='xx-small'),loc = 'best')

plt.savefig("taylor.png", bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')

plt.close(1)

