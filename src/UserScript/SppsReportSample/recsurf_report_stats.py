# -*- coding: cp1252 -*-
##
# Ce fichier permet d'éffectuer des opération de lecture sur un fichier de récepteur de surface
# afin d'établir la répartition des niveaux sonores sur les surfaces
#
import besttick
from libsimpa import rsurf_io,rsurf_data
import math

"""
wxFloat32 to_deciBelRsurf(const wxFloat32& wjVal)
{
	return 10*log10f(wjVal/pow(10.f,-12.f));
}
"""
def TodBRsurf( wj ):
    return 10*math.log10(wj/math.pow(10.,-12.))

##
# Compute min,max of a Surface Receiver
# @return Tuple (min,max) if there is data or (None,None) if there is no face
def GetMinMaxRs( rsData, rsindex ):
    nbfaces, rs_name, xmlid=rsData.GetRsInfo(rsindex)
    if nbfaces==0:
        return (None,None)
    vmin=vmax=rsData.GetFaceSumEnergy(rsindex,0)
    for idface in xrange(nbfaces):
        faceenergy=rsData.GetFaceSumEnergy(rsindex,idface)
        vmin=min(vmin,faceenergy)
        vmax=max(vmax,faceenergy)
    return (vmin,vmax)

#math.floor(((faceenergy-tmin)/(tmax-tmin)*tickcount))
def GetDistributionAreaBySoundLevel( rsData, rsindex, arr_ticks):
    tmin=float(arr_ticks[0])
    tmax=float(arr_ticks[-1])
    tickcount=len(arr_ticks)
    nbfaces, rs_name, xmlid=rsData.GetRsInfo(rsindex)
    area_distribution=[0. for i in range(tickcount)]
    for idface in xrange(nbfaces):
        faceenergy=rsData.GetFaceSumEnergy(rsindex,idface)
        if faceenergy!=0 and not math.isnan(faceenergy):
            face_spl=TodBRsurf(faceenergy)
            area_distribution[int(math.floor(((face_spl-tmin)/(tmax-tmin))*(tickcount-1)))]+=rsData.ComputeFaceArea(rsindex,idface)            
    return area_distribution
##
# Compute the distribution of sound level over the area of each surface receiver
# @return Dictionnary { Surface receiver label : [ [Range labels] [ values_area_in_square_meter ] ]  }
# Uses data=BuildSoundLevelDistributionArray("rs_cut.csbin")
def BuildSoundLevelDistributionArray( surfaceReceiverFilePath ):
    rsData=rsurf_data()
    rsurf_io.Load(surfaceReceiverFilePath.encode('cp1252'),rsData)
    rs_size,nodes_size,nbtimestep,timestep,recordType = rsData.GetFileInfos()
    SoundLevelDistribution={}
    for rsid in range(rs_size):
        nbfaces,rs_name,xmlid = rsData.GetRsInfo(rsid)
        rs_name=rs_name.decode('cp1252')
        rs_arr_distrib=[]
        #Compute min max
        minrs,maxrs=GetMinMaxRs(rsData,rsid)
        if not minrs is None and not maxrs is None and minrs!=maxrs and not minrs==0:
            arr_ticks=besttick.GetTicksFromMinMax(TodBRsurf(minrs),TodBRsurf(maxrs))
            #Ajoute une borne supérieure et inférieure pour contenir toute les valeurs
            arr_ticks.insert(0,arr_ticks[0]-(arr_ticks[1]-arr_ticks[0]))
            arr_ticks.append(arr_ticks[-1]+(arr_ticks[-1]-arr_ticks[-2]))
            #distribue les cumul de surface en fonction du niveau sonore
            area_distribution=GetDistributionAreaBySoundLevel(rsData,rsid,arr_ticks)[:-1]
            #Créé les libellés de la série
            arr_ticks_lbl=[]
            for idtick,spl_tick in enumerate(arr_ticks[:-1]):
                arr_ticks_lbl.append("[%g-%g[" % (spl_tick,arr_ticks[idtick+1]))         
            rs_arr_distrib=[ arr_ticks_lbl , area_distribution ]
            rs_name_out=rs_name
            rs_count=1
            while SoundLevelDistribution.has_key(rs_name_out):
                rs_name_out=rs_name+" "+str(rs_count)
                rs_count+=1
            SoundLevelDistribution[rs_name_out]=rs_arr_distrib
    return SoundLevelDistribution