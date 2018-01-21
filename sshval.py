import os,sys
import numpy as np 
import pandas as pd
import requests as req
import matplotlib.pyplot as plt
import json
import datetime
reload(sys)
sys.setdefaultencoding('utf8')


clegend = []
vexpr   = []        
cstyle  = [] 

def readConfig():

    if len(sys.argv) != 2:
        print "usage "+sys.argv[0]+" config file"
        sys.exit(1)
        
    config_file = sys.argv[1]
    """Read and parse config file"""
    if not os.path.isfile(config_file):
        print 'specified config file {} does not exist.'.format(config_file)
        sys.exit(1)

    with open(config_file, 'r') as fptr_cfg:
        cfg = json.load(fptr_cfg)
        station   = cfg["station"]
        startdate = cfg["startdate"]
        enddate   = cfg["endate"]
        obstyle   = cfg["obstyle"]
        expname   = cfg["experiment"]
        oper      = cfg["operational"]
        tickint   = cfg["tickinterval"]
        return station, startdate, enddate,obstyle, expname, oper, tickint 

def strDate(day):
    cyear, cmonth, cday, chour="%04d"%int(day.year),"%02d"%int(day.month),"%02d"%int(day.day),"%02d"%int(day.hour)
    return "".join([cyear, cmonth, cday,chour])


def getDateRange(fromt, tot):
    initd = datetime.datetime(int(fromt[0:4]),int(fromt[4:6]),int(fromt[6:8]),int(fromt[8:10]))
    totd  = datetime.datetime(int(tot[0:4]),int(tot[4:6]),int(tot[6:8]),int(tot[8:10]))
    step  = datetime.timedelta(hours=1)
    dateticks = []
    while initd <= totd:
        dateticks.append(strDate(initd)) 
        initd = initd + step
    
    return dateticks
    
def fillObs(date_obs,fromt, tot):
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
    
def biasMean(vobs, vmodel):
    print "model shape", vmodel.shape[0]
    print "obs shape", vobs.shape[0]
    if vobs.shape[0] != vmodel.shape[0] :
        print "vobs and vmodel should have the same size"
        sys.exit()
    return np.mean(vobs- vmodel)

def readObs(station, startdate,enddate,obstyle):
    param   = {'from': startdate, 'too': enddate}
    res    = req.get("http://oceandata.smhi.se/ssh/"+station+"/OBSERVATION",params = param)
    dobs     = res.json()
    date_obs = dobs.keys()
    dmiss=fillObs(date_obs,startdate, enddate)
    if dmiss.keys():
        dobs.update(dmiss)
    vo   = pd.DataFrame.from_dict(dobs,orient="index")
    vob  = vo.loc[startdate:enddate].values
    vobs = np.ma.masked_where(np.array(vob) == -999.0, np.array(vob))
    clegend.append("OBSERVATION")
    vexpr.append(vobs)
    cstyle.append(obstyle) 
        
    return vobs

def readExpr(expname,station,startdate,enddate,vobs):
    print expname
    
    for model in expname:
        print model
        dmod  = pd.read_csv(expname[model]["path"])
        date = map(lambda x : str(x), dmod["datetime"].values)
        vm     = pd.Series(dmod[station].values,index=date)
        vmod  = vm.loc[startdate:enddate].values
        vmod  = vmod + biasMean(vobs,vmod)
        clegend.append(model)
        vexpr.append(vmod)
        cstyle.append(expname[model]["style"]) 
        
def readOper(oper,station,startdate,enddate,vobs):
    param   = {'from': startdate, 'too': enddate}
    print oper.keys()
    for model in oper:
        print model
        res1   = req.get("http://oceandata.smhi.se/ssh/"+station+"/"+model,params = param)
        doper  = res1.json()
        vm     = pd.DataFrame.from_dict(doper,orient="index")
        voper  = vm["raw"].loc[startdate:enddate].values
        voper  = voper + biasMean(vobs,voper)
        print type(model)
        clegend.append(oper[model]["title"])
        vexpr.append(voper)
        cstyle.append(oper[model]["style"]) 
##############################

def main():

    print readConfig()

    station, startdate, enddate, obstyle, expname, oper,tickint = readConfig()

    vobs = readObs(station, startdate, enddate,obstyle)
    readExpr(expname,station,startdate,enddate,vobs)
    readOper(oper,station,startdate,enddate,vobs)

    stick = getDateRange(startdate,enddate)
    itick = range(len(stick))
    
    fig=plt.figure(num=1,figsize=(12.0,6.0),dpi=300,facecolor='w',edgecolor='k')
    ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
    ax.set_xlabel('Time',fontsize='20',weight='bold')
    ax.set_ylabel ('SSH(m)',fontsize='20',weight='bold')

    for im in range(len(vexpr)):
        ax.plot(vexpr[im],cstyle[im])

    ax.legend(clegend, loc="best")
    ax.grid()
    ax.set_title(station,fontsize="30")
    ax.xaxis.set_ticks(itick[::tickint])
    ax.xaxis.set_ticklabels(stick[::tickint],rotation="vertical",fontsize="15")
    plt.savefig("ssh"+station+'.png', bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')
    plt.close(1)

if __name__ == "__main__":
    main()
