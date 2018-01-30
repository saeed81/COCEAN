import os,sys
import numpy as np 
import pandas as pd
import requests as req
import matplotlib.pyplot as plt
import json
import datetime
from taylorDiagram import TaylorDiagram
reload(sys)
sys.setdefaultencoding('utf8')

clegend = []
vssh    = []        
cstyle  = [] 
ccolor  = [] 
rms     = []
cor     = []  
rmsd    = []
def format_time_string(time_str):
    """
    Converts 2017-10-01 20:00 -> 201710012000
    """
    input_format = '%Y-%m-%d %H:%M'
    output_format = '%Y%m%d%H'

    return datetime.datetime.strptime(time_str, input_format).strftime(output_format)

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
    dmcor = {}
    while initd <= totd:
        if strDate(initd) not in date_obs: 
            dmiss[strDate(initd)] = {}
            dmcor[strDate(initd)] = {}
            dmcor[strDate(initd)]["raw"] = -999.0
            dmiss[strDate(initd)]["raw"] = np.nan
                        
        initd = initd + step
    return dmcor

def biasMean(vobs, vmodel):
    
    if vobs.shape[0] != vmodel.shape[0] :
        print "vobs and vmodel should have the same size"
        sys.exit()
    
    return np.mean(vobs- vmodel)


def readConfig():

    if len(sys.argv) != 2:
        print "usage "+sys.argv[0]+" config file"
        sys.exit(1)
        
    config_file = sys.argv[1]
    if not os.path.isfile(config_file):
        print 'specified config file {} does not exist.'.format(config_file)
        sys.exit(1)

    with open(config_file, 'r') as fptr_cfg:
        cfg = json.load(fptr_cfg)
        server       = cfg["server"]
        dovalidation = cfg["dovalidation"]
        station      = cfg["station"]
        startdate    = format_time_string(cfg["startdate"])
        enddate      = format_time_string(cfg["endate"])
        obstyle      = cfg["obstyle"]
        expname      = cfg["experiment"]
        oper         = cfg["operational"]
        tickint      = cfg["tickinterval_hour"]
        return server,dovalidation, station, startdate, enddate,obstyle, expname, oper, tickint 
    

def readObs(server,station, startdate,enddate,obstyle):
    if server["prod"]: param   = {'from': startdate, 'too': enddate}
    if server["utv"]:param   = {'highfreq':'false','from': startdate, 'too': enddate}
    if server["prod"]: res     = req.get("http://oceandata.smhi.se/ssh/"+station+"/OBSERVATION",params = param)
    if server["utv"]:res     = req.get("http://oceandata-utv.smhi.se/ssh/"+station+"/OBSERVATION",params = param)
    dobs    = res.json()
    date_obs = dobs.keys()
    dmiss = fillObs(date_obs,startdate, enddate)
    if dmiss.keys():
        dobs.update(dmiss)
    
    voc   = pd.DataFrame.from_dict(dobs,orient="index")
    vobc  = voc.loc[startdate:enddate].values
    vobsc = np.squeeze(vobc)
    vobsm = np.ma.masked_where(vobsc==-999.0,vobsc)
    obstd  = np.std(vobsm)
    rms.append(round(obstd,3))
    cor.append(1.0)
    rmsd.append(0.0)
    clegend.append("OBSERVATION")
    vssh.append(vobsm)
    cstyle.append(obstyle["line"]) 
    ccolor.append(obstyle["color"]) 
        
    return vobsm
    
def readExpr(expname,station,startdate,enddate,vobsm):
        
    for model in expname:
        dmod   = pd.read_csv(expname[model]["path"])
        date   = map(lambda x : str(x), dmod["datetime"].values)
        vm     = pd.Series(dmod[station].values,index=date)
        vmod   = vm.loc[startdate:enddate].values
        vmod   = vmod + biasMean(vobsm,vmod)
        lcorr  = np.ma.corrcoef(vmod, vobsm)[0,1]
        lrms   = np.sqrt(np.mean((vmod - vobsm)**2),dtype=np.float64)
        lrms   = np.sqrt(((vmod-np.mean(vmod)-vobsm+ np.mean(vobsm))**2).mean())
        obstd  = np.std(vobsm)
        difstd = np.std(vmod - vobsm)
        modstd = np.std(vmod)
        rms.append(round(modstd,3))
        cor.append(round(lcorr,3))
        rmsd.append(round(difstd,3))
        clegend.append(model)
        vssh.append(vmod)
        cstyle.append(expname[model]["line"]) 
        ccolor.append(expname[model]["color"]) 
        
def readOper(server,oper,station,startdate,enddate,vobsm):
    if server["prod"]:param   = {'from': startdate, 'too': enddate}
    if server["utv"]: param   = {'highfreq':'false','from': startdate, 'too': enddate}
    for model in oper:
        if server["prod"]:res1   = req.get("http://oceandata.smhi.se/ssh/"+station+"/"+model,params = param)
        if server["utv"]:res1   = req.get("http://oceandata-utv.smhi.se/ssh/"+station+"/"+model,params = param)
        doper  = res1.json()
        vm     = pd.DataFrame.from_dict(doper,orient="index")
        voper  = vm["raw"].loc[startdate:enddate].values
        voper  = voper + biasMean(vobsm,voper)
        lcorr  = np.ma.corrcoef(voper, vobsm)[0,1]
        lrms   = np.sqrt(np.mean((voper - vobsm)**2),dtype=np.float64)
        lrms   = np.sqrt(((voper-np.mean(voper)-vobsm+ np.mean(vobsm))**2).mean())
        lrms   = np.sqrt(((voper-vobsm)**2).mean())
        difstd = np.std(voper - vobsm)
        modstd = np.std(voper)
        obstd  = np.std(vobsm)
        rms.append(round(modstd,3))
        cor.append(round(lcorr,3))
        rmsd.append(round(difstd,3))
        clegend.append(oper[model]["title"])
        vssh.append(voper)
        cstyle.append(oper[model]["line"])
        ccolor.append(oper[model]["color"])
##############################
def main():

    server,dovalidation, station, startdate, enddate, obstyle, expname, oper,tickint = readConfig()

    vobsm = readObs(server,station, startdate, enddate,obstyle)
    if dovalidation["experiment"]  :readExpr(expname,station,startdate,enddate,vobsm)
    if dovalidation["operational"] :readOper(server,oper,station,startdate,enddate,vobsm)

    stick = getDateRange(startdate,enddate)
    itick = range(len(stick))
    iitick=itick[::tickint]

    fig=plt.figure(num=1,figsize=(12.0,6.0),dpi=300,facecolor='w',edgecolor='k')
    ax = fig.add_axes([0.08, 0.08, 0.8, 0.8], axisbg = '1.0')
    ax.set_xlabel('Time',fontsize='20',weight='bold')
    ax.set_ylabel ('SSH(m)',fontsize='20',weight='bold')

    for im in range(len(vssh)):
        ax.plot(vssh[im],linestyle=cstyle[im],color=ccolor[im],lw=0.75)

    ax.legend(clegend, numpoints=1, prop=dict(size='small'),loc="best")
    ax.grid()
    ax.set_title(station,fontsize="30")
    ax.xaxis.set_ticks(itick[::tickint])
    ax.set_xlim(iitick[0],iitick[-1])
    ax.xaxis.set_ticklabels(stick[::tickint],rotation="vertical",fontsize="15")
    print "Creating figure for station", station, " ssh"+station+'.png'
    plt.savefig("ssh"+station+'.png', bbox_inches='tight',dpi=300,facecolor='w',edgecolor='w',orientation='portrait')
    plt.close(1)

    
    TaylorDiagram(RMSVEC=rms, RMSDVEC=rmsd, CORVEC=cor,COLORVEC=ccolor,LABELVEC=clegend, station=station, info=startdate+"-"+enddate)
        
if __name__ == "__main__":
    main()
