# -*- coding: cp1252 -*-

import uictrl as ui
import uilocale
from libsimpa import *
ScriptFolder=ui.application.getapplicationpath()["userscript"]+"SppsReportSample\\"
import recsurf_report_stats
import os
import math
import csv

_=uilocale.InstallUiModule(ScriptFolder,ui.application.getlocale())

#Use data=SppsReportSample.GetPonctualReceiversData(wxid)
def ReplaceNanByZero(el):
    try:
        if math.isnan(el):
            return 0.
        else:
            return el
    except:
        return el
def GetPonctualReceiversData(folderwxid):
    """
     Retourne un dictionnaire contenant les données des récepteurs ponctuels
     Structure
     { "Libellé du récepteur ponctuel"
       : { "sound_level" : [],
           "rt30" : [],
           "edt" : []

         }
     }
    """
    data={}
    #folder devient l'objet dossier
    folder=ui.element(folderwxid)
    #dans un tableau on place les indices des fichiers de données des récepteurs ponctuels
    recplist=folder.getallelementbytype(ui.element_type.ELEMENT_TYPE_REPORT_GABE_RECP)
    #Pour chaque récepteur on demande a l'application les données traitées du fichier ( niveau sonore et cumuls )
    for idrecp in recplist:
        #recp devient l'objet ayant comme indice idrecp (entier)
        recp=ui.element(idrecp)
        recpinfos=recp.getinfos()
        if recpinfos["name"]=="soundpressure":
            recpdata={}
            #on recupere l'element parent (le dossier de récepteur ponctuel)
            pere=ui.element(recp.getinfos()["parentid"])

            #on demande le calcul des paramètres sonores
            if pere.getelementbylibelle('acoustic_param')==-1:
                ui.application.sendevent(recp,ui.idevent.IDEVENT_RECP_COMPUTE_ACOUSTIC_PARAMETERS,{"TR":"30"})

            #application.sendevent(pere,idevent.IDEVENT_RELOAD_FOLDER)
            nomrecp=pere.getinfos()["label"]
            #on recupere les données calculées
            params=ui.element(pere.getelementbylibelle('acoustic_param'))
            #on stocke dans gridspl le tableau des niveaux de pression
            gridparam=ui.application.getdataarray(params)
            rotated=zip(*gridparam)
            labels=gridparam[0]
            tr30index=labels.index(ui._("TR-%g (s)") % (30))
            edt_index=labels.index(ui._("EDT (s)"))
            #on ajoute les données
            recpdata["sound_level"]=ui.application.getdataarray(recp)
            recpdata["rt30"]=map(ReplaceNanByZero,rotated[tr30index])
            recpdata["edt"]=map(ReplaceNanByZero,rotated[edt_index])
            data[nomrecp]=recpdata
    return data
def SaveDataToCSV(pr_data,workingdir):
    #Sauvegarde du niveau sonore
        rplbl_lst=[]
        spl_byfreq=[]
        spl_html_array=u""
        csv_rows=[]
        if len(pr_data)!=0:
            #####
            # Sound level

            freq_lst=[ col[0] for col in pr_data[pr_data.keys()[0]]["sound_level"][1:]]
            spl_byfreq=[[] for i in range(len(freq_lst))]
            for ponctualreceiver_name, rp_data in buildhtmlreport.SortedDictByKeys(pr_data):
                rplbl_lst.append(ponctualreceiver_name)
                for idfreq,value in enumerate([ line[-1] for line in rp_data["sound_level"][1:]]):
                    spl_byfreq[idfreq].append(value)
            #spl_html_array+="<tr><th>%s</th>" % (_("SPL"))
            header=["SPL"]
            for rplbl in rplbl_lst:
                header.append(rplbl)
            csv_rows.append(header)
            for freqlbl, prs_spl in zip(freq_lst, spl_byfreq):
                line=[freqlbl]
                for rp_spl_val in prs_spl:
                    line.append(str(rp_spl_val).replace(".",","))
                csv_rows.append(line)
            csv_rows.append([])
            #####
            # reverberation time
            tr_byfreq=[[] for i in range(len(freq_lst))]
            for ponctualreceiver_name, rp_data in buildhtmlreport.SortedDictByKeys(pr_data):
                for idfreq,value in enumerate(rp_data["rt30"][1:-1]):
                    tr_byfreq[idfreq].append(value)
            header=["RT30"]
            for rplbl in rplbl_lst:
                header.append(rplbl)
            csv_rows.append(header)
            for freqlbl, prs_tr in zip(freq_lst, tr_byfreq):
                line=[freqlbl]
                for rp_tr_val in prs_tr:
                    line.append(str(rp_tr_val).replace(".",","))
                csv_rows.append(line)
            #write to file
            spl_csv_writer=csv.writer(open(workingdir+workingdir.split("\\")[-2]+".csv","w"), delimiter=";", quoting=csv.QUOTE_NONE,lineterminator="\n")
            for line in csv_rows:
                spl_csv_writer.writerow(line)

def MakeHtmlReport(folderwxid):
    #Actualisation de l'interface
    import buildhtmlreport
    buildhtmlreport._ = _ #surcharge de la méthode de traduction
    sppsreportbuilder=buildhtmlreport.ReportBuilder()

    mainfolder=ui.e_file(folderwxid)   #instance du dossier
    ui.application.sendevent(mainfolder,ui.idevent.IDEVENT_RELOAD_FOLDER)
    subelementsid=mainfolder.childs()   #Recuperation des sous dossiers et fichiers
    rotated=zip(*subelementsid) #Inversion lignes, colonnes

    #Recuperation des données pour les récepteurs ponctuels
    prfolder=rotated[0][rotated[2].index(u'Récepteurs_Ponctuels')]
    pr_dict=GetPonctualReceiversData(prfolder)
    #Recuperation des données pour les récepteurs surfacique
    if 'recepteurss' in rotated[2]:
        mainrsfold=ui.e_file(rotated[0][rotated[2].index(u'recepteurss')]).buildfullpath()+"\\Global\\"
        rscutpath=mainrsfold+"rs_cut.csbin"
        rs_data={}
        if os.path.exists(rscutpath):
            rs_data.update(recsurf_report_stats.BuildSoundLevelDistributionArray(rscutpath))
        rs_modelpath=mainrsfold+"Niveau_Sonore.csbin"
        if os.path.exists(rs_modelpath):
            rs_data.update(recsurf_report_stats.BuildSoundLevelDistributionArray(rs_modelpath))
        sppsreportbuilder.SetSurfaceReceiverData(rs_data)

    #Generation du rapport
    sppsreportbuilder.SetPonctualReceiverData(pr_dict)

    #Sauvegarde au format CSV
    SaveDataToCSV(pr_dict,mainfolder.buildfullpath())

    #Sauvegarde du rapport
    sppsreportbuilder.BuildHtmlCode(ScriptFolder,mainfolder.buildfullpath(),ui.application.getlocale())

    #Actualisation de l'interface
    ui.application.sendevent(mainfolder,ui.idevent.IDEVENT_RELOAD_FOLDER)
class manager:
    def __init__(self):
        self.MakeReportEventId=ui.application.register_event(self.OnMakeHtmlReport)
    def getmenu(self,typeel,idel,menu):
        #Si le pere de l'element se nomme SPPS
        el=ui.element(idel)
        infos=el.getinfos()
        if infos["parentid"]>0:
            parent=ui.element(infos["parentid"])
            if parent.getinfos()["label"]=="SPPS" or parent.getinfos()["label"]==u"Modèle de diffusion":
                menu.insert(0,(_("Make report"),self.MakeReportEventId))
                return True
            else:
                return False
        return False
    def OnMakeHtmlReport(self,elid):
        MakeHtmlReport(elid)
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_REPORT_FOLDER, manager())
