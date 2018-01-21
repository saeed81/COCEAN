import os,sys
import numpy as np 
import pandas as pd
import requests as req
import matplotlib.pyplot as plt
import json
import datetime

def strDate(day):
    cyear, cmonth, cday, chour="%04d"%int(day.year),"%02d"%int(day.month),"%02d"%int(day.day),"%02d"%int(day.hour)
    return "".join([cyear, cmonth, cday,chour])

def fill_obs(date_obs,fromt, tot):
    initd = datetime.datetime(int(fromt[0:4]),int(fromt[4:6]),int(fromt[6:8]),int(fromt[8:10]))
    totd  = datetime.datetime(int(tot[0:4]),int(tot[4:6]),int(tot[6:8]),int(tot[8:10]))
    step  = datetime.timedelta(hours=1)
    dmiss = {}
    while initd <= totd:
        if strDate(initd) not in date_obs: 
            dmiss[strDate(initd)] = {}
            dmiss[strDate(initd)]["raw"] = -999.0
            
        initd = initd + step
    return dmiss
    
def bias_mean(vobs, vmodel):
    print "model shape", vmodel.shape[0]
    print "obs shape", vobs.shape[0]
    if vobs.shape[0] != vmodel.shape[0] :
        print "vobs and vmodel should have the same size"
        sys.exit()
    return np.mean(vobs- vmodel)

##############################
station = "barseback"
sfrom   = '2017100100'
stoo    = '2017102900'
param   = {'from': sfrom, 'too': stoo}

#read observation
res    = req.get("http://oceandata.smhi.se/ssh/"+station+"/OBSERVATION",params = param)
#res   = req.get("http://oceandata.smhi.se/ssh/"+station+"/OBSERVATION?"+"from="+sfrom+"&too="+stoo)
#print res.content
dobs     = res.json()
print len(dobs.keys())
date_obs = dobs.keys()
dmiss=fill_obs(date_obs,sfrom, stoo)
if dmiss.keys():
    dobs.update(dmiss)
print len(dobs.keys())
vo      = pd.DataFrame.from_dict(dobs,orient="index")
vob     = vo.loc[sfrom:stoo].values
vobs   = np.ma.masked_where(np.array(vob) == -999.0, np.array(vob))

#read model data 1 experiment
dmod1  = pd.read_csv("ssh_ns6_minh_4.csv")
date   = map(lambda x : str(x), dmod1["datetime"].values)
v1     = pd.Series(dmod1[station].values,index=date)
vmod1  = v1.loc[sfrom:stoo].values
vmod1  = vmod1 + bias_mean(vobs,vmod1)

#read model data 2 experiment
dmod2  = pd.read_csv("ssh_ns6_minh_4_correct_sss.csv")
date   = map(lambda x : str(x), dmod2["datetime"].values)
v2     = pd.Series(dmod2[station].values,index=date)
vmod2  = v2.loc[sfrom:stoo].values
vmod2  = vmod2 + bias_mean(vobs,vmod2)

##read operational
res1   = req.get("http://oceandata.smhi.se/ssh/"+station+"/NS01_NEMO",params = param)
doper  = res1.json()
vm     = pd.DataFrame.from_dict(doper,orient="index")
voper  = vm["raw"].loc[sfrom:stoo].values
voper  = voper + bias_mean(vobs,voper)

###plotting
ldate  = dict(v2.loc[sfrom:stoo]).keys()
idate  = [int(vd) for vd in ldate]
sidate = sorted(idate)

itick = range(len(sidate))
ctick = sidate
stick = 48

fig=plt.figure(num=1,figsize=(12.0,6.0),dpi=300,facecolor='w',edgecolor='k')
ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
ax.set_xlabel('Time',fontsize='20',weight='bold')
ax.set_ylabel ('SSH(m)',fontsize='20',weight='bold')
ax.plot(vobs,'r-',vmod2,'b-',voper,'g-')
ax.legend(["OBS",'NEMO_NS01_GEBCO_northsea_6m_mindepth_4m',"NEMO_NS01_OPER"], loc="best")
#ax.set_ylim(-1.0,1.0)
ax.grid()
ax.set_title(station,fontsize="30")
ax.xaxis.set_ticks(itick[::stick])
ax.xaxis.set_ticklabels(ctick[::stick],rotation="vertical",fontsize="15")
plt.savefig("ssh"+station+'.png', bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')
plt.close(1)

