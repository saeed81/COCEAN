def TaylorDiagram(RMSVEC, CORVEC,COLORVEC,LABELVEC, station, info):
    import numpy as np
    import matplotlib.pyplot as plt
    import math

    rms_max = float(math.ceil(max(RMSVEC)))
    if rms_max <= 1.0:                    delta = 0.2  
    if rms_max >= 2.0 and rms_max <= 3.0: delta = 0.25  
    if rms_max >=4 and rms_max <= 5.0:    delta = 0.5  
    if rms_max >=6 and rms_max <= 7.0:    delta = 0.5  
    if rms_max >=8 and rms_max <= 9.0:    delta = 1.0  
    if rms_max >=10:                      delta = 1.0  
    
    X=np.arange(0.0,rms_max+delta/100.0,delta/100.0)
    Y=np.arange(0.0,rms_max+delta/100.0,delta/100.0)
    nx=X.shape[0]
    ny=Y.shape[0]
    h = np.zeros((ny,nx),dtype=np.float32)
    for j in range(ny):
        for i in range(nx):
            h[j,i] = math.sqrt(X[i] * X[i] + Y[j] * Y[j])

    fig=plt.figure(num=1,figsize=(9.0,9.0),dpi=300,facecolor='w',edgecolor='k')
    ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
    ax.set_xlabel('RMSE/STD_OBS',fontsize='15',weight='bold',color="green")
    ax.set_ylabel('RMSE/STD_OBS',fontsize='15',weight='bold',color="green")
    ax.grid(False)
    
    vc=np.arange(0.0,rms_max+delta,delta)

    ax.contour(X,Y,h,vc,colors='0.5',linewidths=0.2)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_xlim(0.0,rms_max + 0.005)
    ax.set_ylim(0.0,rms_max + 0.005)
    ax.yaxis.set_ticks_position('left')
    ax.xaxis.set_ticks_position('bottom')

    radius = np.arange(0.0,rms_max + delta,delta)
    xangle = list(np.arange(0.0,0.9,0.10)) + [0.9, 0.95, 0.99]
    
    for rd in radius:  
        for ang in xangle:
            ax.plot([0.0,rd*ang],[0.0,rd*math.sqrt(1.0 - (ang * ang))],color="0.5",ls="-",lw="0.10")
            if rd == rms_max and ang > 0.0: ax.text(rd*ang, rd*math.sqrt(1.0 - (ang * ang)), str(ang),fontsize=10) 
                
    ang = 0.65
    ax.text((1.040)*rms_max*ang, (1.04)*rms_max*math.sqrt(1.0 - (ang * ang)), "Correlation",color="Steelblue",fontsize=20,rotation=-45)

    lenm = len(RMSVEC) 
    
    for ii in range(lenm):
        vrms, vcor, vcol, vlab =  RMSVEC[ii], CORVEC[ii], COLORVEC[ii],LABELVEC[ii]
        if ii == 0 : 
            #line, = ax.plot(vrms*vcor,vrms*math.sqrt(1.0 - (vcor * vcor)),'o',color=vcol, label=vlab,ms=8)
            #line.set_clip_on(False)
            ax.text(rms_max/10.0, -rms_max/8.0, "STD_OBS  "+str(vrms),color="red",fontsize=12,rotation=0)
        else:ax.plot(vrms*vcor,vrms*math.sqrt(1.0 - (vcor * vcor)),'o',color=vcol, label=vlab,ms=8)

    ax.legend(numpoints=1,loc = 'best',prop=dict(size='small'),fontsize=12)
    ax.set_title(station+" "+info,fontsize="20")
    plt.savefig("taylor"+station+".pdf", bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')
    plt.close(1)


def main():
    rms = [0.4,2.4,2.1,2.9]
    cor = [0.5,0.25,0.76,0.24]
    lab = ["exp1","exp2","exp3","exp4"]
    col = ["b","g","r","k"]
    TaylorDiagram(RMSVEC=rms, CORVEC=cor,COLORVEC=col,LABELVEC=lab, station="viken")

if __name__ == "__main__":main()
    
