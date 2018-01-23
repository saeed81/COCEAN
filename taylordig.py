import numpy as np
import matplotlib.pyplot as plt
import math

X=np.arange(0.0,2.1,0.001)
Y=np.arange(0.0,2.1,0.001)

print X.shape 

print Y.shape

nx=X.shape[0]
ny=Y.shape[0]

h = np.zeros((ny,nx),dtype=np.float32)

for j in range(ny):
    for i in range(nx):
        h[j,i] = math.sqrt(X[i] * X[i] + Y[j] * Y[j])

fig=plt.figure(num=1,figsize=(12.0,6.0),dpi=300,facecolor='w',edgecolor='k')
ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
ax.set_xlabel('RMS/OBS_STD',fontsize='20',weight='bold')
ax.set_ylabel ('RMS/OBS_STD',fontsize='20',weight='bold')
ax.grid(False)
vc=np.arange(0.0,2.2,0.2)

ax.contour(X,Y,h,vc,colors='0.5')
#ax.spines['top'].set_visible(False)
#ax.spines['right'].set_visible(False)
ax.set_xlim(0.0,2.0)
ax.set_ylim(0.0,2.0)

radius = np.arange(0.0,5.0,0.5)
xangle = [1.0,0.99,0.95] + list(np.arange(0.9,-0.1,-0.1))

#print xangle

for ang in xangle:
    print ang, math.degrees(math.acos(ang))

for rd in radius:  
    for ang in xangle:
        #print math.cos(math.acos(ang))
        if ang == 0.99 or ang == 0.95:
            ax.plot([0.0,rd*math.cos(math.acos(ang))],[0.0,rd*math.sin(math.acos(ang))],'k-',lw="0.0")
        else:
            ax.plot([0.0,rd*math.cos(math.acos(ang))],[0.0,rd*math.sin(math.acos(ang))],'k-',lw="0.25")
        

ax.plot(0.2*math.cos(math.acos(0.5)),0.2*math.sin(math.acos(0.5)),'ob',ms=3)

plt.savefig("taylor.png", bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')

plt.close(1)

