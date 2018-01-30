def TaylorDiagram(RMSVEC, RMSDVEC, CORVEC,COLORVEC,LABELVEC, station, info):
    import numpy as np
    import matplotlib.pyplot as plt
    import math

    print "name",LABELVEC
    print "cor",CORVEC
    print "rms ",RMSVEC 
    print "rmsd",RMSDVEC

    rms_max = max(RMSVEC)
    delta = rms_max/10.0
    X=np.arange(0.0,(1.20)*rms_max+delta/100.0,delta/100.0)
    Y=np.arange(0.0,(1.20)*rms_max+delta/100.0,delta/100.0)
    nx=X.shape[0]
    ny=Y.shape[0]
    h = np.zeros((ny,nx),dtype=np.float32)
    for j in range(ny):
        for i in range(nx):
            h[j,i] = math.sqrt(X[i] * X[i] + Y[j] * Y[j])
        
    ##########################################
    rmsd_max = max(RMSDVEC)
    ddelta = rmsd_max/10.0
    XX=np.arange(0.0,(1.20)*rms_max+delta/100.0,delta/100.0)
    YY=np.arange(0.0,(1.20)*rms_max+delta/100.0,delta/100.0)
    nxx=XX.shape[0]
    nyy=YY.shape[0]
    hh = np.zeros((nyy,nxx),dtype=np.float32)
    hh[:,:] = -1.0
    for j in range(nyy):
        for i in range(nxx):
            if (math.sqrt((XX[i])*(XX[i])+ YY[j]*YY[j]) <= (1.20*rms_max)):hh[j,i] = math.sqrt((XX[i]-RMSVEC[0]) * (XX[i]-RMSVEC[0]) + YY[j] * YY[j])
    hh=np.ma.masked_where(hh==-1.0,hh)
    ########################################### 
    fig=plt.figure(num=1,figsize=(9.0,9.0),dpi=300,facecolor='w',edgecolor='k')
    ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
    ax.set_xlabel('Standard Deviation',fontsize='15',weight='bold',color="green")
    ax.set_ylabel('Standard Deviation',fontsize='15',weight='bold',color="green")
    ax.grid(False)
    ############################################
    vc=np.arange(0.0,(1.20*rms_max)+delta,delta)
    nl, = vc.shape
    lines=[]
    for i in range(nl-1):
        lines.append("dashed")
    lines.append("solid")
    ax.contour(X,Y,h,vc,colors='0.5',linestyles=lines,linewidths=0.2)
    ############################################
    vvc=np.arange(0.0,rmsd_max+ddelta,ddelta)
    crm=ax.contour(XX,YY,hh,vvc[::2],colors='lightgreen',linestyles="solid",linewidths=1.5)
    ax.clabel(crm,vvc[::2],inline=1,fontsize=12,colors='k',fmt='%.3f') 
    ############################################
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_xlim(0.0,(1.25)*rms_max)
    ax.set_ylim(0.0,(1.25)*rms_max)
    ax.yaxis.set_ticks_position('left')
    ax.xaxis.set_ticks_position('bottom')
    ############################################
    radius = np.arange(0.0,(1.20)*rms_max+delta,delta)
    xangle = list(np.arange(0.0,0.9,0.10)) + [0.9, 0.95, 0.99]
    rdmax = np.amax(radius)
    ############################################
    for rd in radius:  
        for ang in xangle:
            ax.plot([0.0,rd*ang],[0.0,rd*math.sqrt(1.0 - (ang * ang))],color="0.5",ls="-",lw="0.10")
            if rd == rdmax and ang > 0.0: ax.text(rd*ang, rd*math.sqrt(1.0 - (ang * ang)), str(ang),fontsize=10) 
                
    ############################################
    ang = 0.65
    ax.text((1.040)*rdmax*ang, (1.04)*rdmax*math.sqrt(1.0 - (ang * ang)), "Correlation",color="Steelblue",fontsize=20,rotation=-45)
    ############################################
    lenm = len(RMSVEC) 
    for ii in range(lenm):
        vrms, vcor, vcol, vlab =  RMSVEC[ii], CORVEC[ii], COLORVEC[ii],LABELVEC[ii]
        if ii == 0 : 
            line, = ax.plot(vrms*vcor,vrms*math.sqrt(1.0 - (vcor * vcor)),'o',color=vcol, label=vlab,ms=8)
            line.set_clip_on(False)
            #ax.text(rms_max/10.0, -rms_max/8.0, "STD_OBS  "+str(vrms),color="red",fontsize=12,rotation=0)
        else:ax.plot(vrms*vcor,vrms*math.sqrt(1.0 - (vcor * vcor)),'o',color=vcol, label=vlab,ms=8)

    ax.legend(numpoints=1,loc = 'best',prop=dict(size='small'),fontsize=12)
    ax.set_title(station+" "+info,fontsize="20")
    plt.savefig("taylor"+station+".pdf", bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')
    plt.close(1)
    #############################################

def main():
    rms  = [0.4,2.4,2.1,2.9]
    cor  = [0.5,0.25,0.76,0.24]
    rmsd = [0.04,0.24,.21,.29]
    lab  = ["exp1","exp2","exp3","exp4"]
    col  = ["b","g","r","k"]
    TaylorDiagram(RMSVEC=rms, RMSDVEC=rmsd, CORVEC=cor,COLORVEC=col,LABELVEC=lab, station="viken")

if __name__ == "__main__":main()
    
