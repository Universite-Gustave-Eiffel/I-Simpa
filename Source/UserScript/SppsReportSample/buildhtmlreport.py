# -*- coding: cp1252 -*-
import os
import shutil
import codecs
from graphy.backends import google_chart_api
import settings
import besttick
import math
def GoogleEncode(data):
    return data.encode("utf-8")
def supprime_accent(ligne):
    """ supprime les accents du texte source """
    try:
        accents = { 'a': ['à', 'ã', 'á', 'â'],
                    'e': ['é', 'è', 'ê', 'ë'],
                    'i': ['î', 'ï'],
                    'u': ['ù', 'ü', 'û'],
                    'o': ['ô', 'ö'] }
        for (char, accented_chars) in accents.iteritems():
            for accented_char in accented_chars:
                if accented_char in ligne:
                    ligne = ligne.replace(accented_char, char)
    except TypeError:
        accents = { u'a': [u'à', u'ã', u'á', u'â'],
                    u'e': [u'é', u'è', u'ê', u'ë'],
                    u'i': [u'î', u'ï'],
                    u'u': [u'ù', u'ü', u'û'],
                    u'o': [u'ô', u'ö'] }
        for (char, accented_chars) in accents.iteritems():
            for accented_char in accented_chars:
                if accented_char in ligne:
                    ligne = ligne.replace(accented_char, char)        
        return str(ligne)
    return ligne

def _(msg):
    return msg
##
# Si un nombre est présent dans la chaine alors il est extrait de la chaine
def strtodigit(strin):
    digit=""
    for char in strin:
        if char.isdigit():
            digit+=char
    if len(digit)>0:
        return int(digit)
    else:
        return strin

def SortedDictByKeys(adict):
    keys = adict.keys()
    keys.sort(key=strtodigit)
    return [[key, adict[key]] for key in keys]
##
# Replace html source code tags and write the new webpage
def ReplaceHtmlTags(  webpage_path, replace_dict, outwebpage ):
    if not os.path.exists(webpage_path):
        raise("Unable to find the source webpage !")
    #Lecture du ficher html source
    infile=open(webpage_path,'r')
    html_out=codecs.open(outwebpage,'w', "utf-8")
    for line in infile:
        if line.find(u"$")!=-1:
            for keyTag,TagValue in replace_dict.iteritems():
                if line.find(keyTag)!=-1:
                    line=line.replace(u"$"+keyTag,TagValue)
        html_out.write(line)
    infile.close()
    html_out.close()


##
# @param pr_data Ponctual receiver data
# @param sr_data Surface receiver data
# @return Html Code for the synthesis webpage
def MakePageOne( tags_dict, pr_data, sr_data ):
    tags_dict["TITLE_SOUNDLEVEL"]=_("Sound level")
    tags_dict["TITLE_PONCTUAL_RECEIVER"]=_("Punctual receivers")
    tags_dict["TITLE_SURFACE_RECEIVER"]=_("Surface receivers")
    tags_dict["TITLE_ACOUSTIC_PARAMETERS"]=_("Acoustic parameters")
    tags_dict["TITLE_REVERBERATION_TIME"]=_("Reverberation time")
    ##############################################################
    #Mise en page des niveaux sonores
    
    ##
    # Récepteurs ponctuels
    rplbl_lst=[]
    spl_byfreq=[]
    spl_html_array=u""
    if len(pr_data)!=0:
        freq_lst=[ col[0] for col in pr_data[pr_data.keys()[0]]["sound_level"][1:]]
        spl_byfreq=[[] for i in range(len(freq_lst))]
        for ponctualreceiver_name, rp_data in SortedDictByKeys(pr_data):
            rplbl_lst.append(ponctualreceiver_name)
            for idfreq,value in enumerate([ line[-1] for line in rp_data["sound_level"][1:]]):
                spl_byfreq[idfreq].append(value)
        spl_html_array+="<tr><th>%s</th>" % (_("SPL"))
        for rplbl in rplbl_lst:
            spl_html_array+="<th>%s</th>" % rplbl
        spl_html_array+="</tr>\n"
        for freqlbl, prs_spl in zip(freq_lst, spl_byfreq):
            newhtmlline="<th>%s</th>\n" % (freqlbl)
            for rp_spl_val in prs_spl:
                newhtmlline+="<td>%.2f</td>" % (rp_spl_val)                
            spl_html_array+="<tr class=\"even\">"+newhtmlline+"</tr>"
    tags_dict["ARRAY_PR_SOUNDLEVEL"]=spl_html_array
    ####
    #Création du graphique
    if len(spl_byfreq)>0:
        spl_chart = google_chart_api.BarChart()
        spl_ticksBuilder=besttick.AxesLabelsCreator()
        spl_chart.AddBars(spl_byfreq[-1])
        spl_ticksBuilder.feed(spl_byfreq[-1])
        spl_chart.bottom.labels = map(GoogleEncode,rplbl_lst)
        spl_ticksBuilder.setmargin()
        spl_chart.left.min = spl_ticksBuilder.min
        spl_chart.left.max = spl_ticksBuilder.max
        spl_chart.left.labels = spl_ticksBuilder.GetTicksLabels("%.2f dB")
        spl_chart.left.label_positions = spl_ticksBuilder.GetTicksPosition()
        spl_chart.left.label_gridlines = True
        spl_chart.display.extra_params['chf'] = "bg,s,65432100"
        tags_dict["CHART_SPL"]=spl_chart.display.Img(settings.width, settings.height)
    ##
    # Récepteurs surfacique
    
    #Création du graphique
    tags_dict["CHART_SR"]=""
    for rs_name,rs_tuple in SortedDictByKeys(sr_data):
        sr_chart=google_chart_api.BarChart()
        sr_ticksBuilder=besttick.AxesLabelsCreator()
        sr_chart.AddBars(rs_tuple[1])
        sr_ticksBuilder.feed(rs_tuple[1])
        sr_chart.bottom.labels = rs_tuple[0]
        sr_ticksBuilder.setmargin()
        sr_chart.left.min = sr_ticksBuilder.min
        sr_chart.left.max = sr_ticksBuilder.max
        
        area_ticks=sr_ticksBuilder.GetTicksPosition()
        sumArea=float(sum(rs_tuple[1]))
        percentage_ticks=[ area/sumArea*100. for area in area_ticks]
        left_labels=[ "%g m2 (%.f %%)" % (area,percentage) for area,percentage in zip(area_ticks,percentage_ticks)]
        sr_chart.left.labels = left_labels
        sr_chart.left.label_positions = area_ticks
        sr_chart.left.label_gridlines = True
        sr_chart.display.extra_params['chf'] = "bg,s,65432100"
        sr_chart.display.extra_params['chtt'] = GoogleEncode(rs_name)
        tags_dict["CHART_SR"]+="<br />"+sr_chart.display.Img(settings.width, settings.height)
        
    ##############################################################
    #Mise en page des paramètres acoustiques
    rt_html_array=u""
    if len(pr_data)!=0:
        freq_lst=[ col[0] for col in pr_data[pr_data.keys()[0]]["sound_level"][1:]]
        rplbl_lst=[]
        tr_byfreq=[[] for i in range(len(freq_lst))]
        for ponctualreceiver_name, rp_data in SortedDictByKeys(pr_data):
            rplbl_lst.append(ponctualreceiver_name)
            for idfreq,value in enumerate(rp_data["rt30"][1:-1]):
                tr_byfreq[idfreq].append(value)
                #print "name: %s idfreq:%i value:%f" % (ponctualreceiver_name,idfreq,value)
        rt_html_array+="<tr><th>%s</th>" % (_("RT30"))
        for rplbl in rplbl_lst:
            rt_html_array+="<th>%s</th>" % (rplbl)
        rt_html_array+="</tr>\n"
        for freqlbl, prs_tr in zip(freq_lst, tr_byfreq):
            newhtmlline="<th>%s</th>\n" % (freqlbl)
            for rp_tr_val in prs_tr:
                newhtmlline+="<td>%.2f</td>" % (rp_tr_val)                
            rt_html_array+="<tr class=\"even\">"+newhtmlline+"</tr>"
    tags_dict["ARRAY_RT30"]=rt_html_array
    ####
    #Création du graphique
    chart = google_chart_api.BarChart()
    display_mode="byfreq"
    if len(pr_data)>5: #si il y a plus de 5 récepteurs ponctuels on limite à la valeur globale
        display_mode="global_only"
    
    ticksBuilder=besttick.AxesLabelsCreator()
    if display_mode=="byfreq":
        for ponctualreceiver_name, rp_data in SortedDictByKeys(pr_data):
            bar_data=rp_data["rt30"][1:]
            chart.AddBars(bar_data, label=GoogleEncode(ponctualreceiver_name)) #, color='ccccff'
            ticksBuilder.feed(bar_data)
        if len(pr_data)!=0:
            chart.bottom.labels = [ col[0] for col in pr_data[pr_data.keys()[0]]["sound_level"][1:]]
    else:
        rplist=[]
        rplvllist=[]
        for ponctualreceiver_name, rp_data in SortedDictByKeys(pr_data):
            rplvllist.append(rp_data["rt30"][-1])
            rplist.append(GoogleEncode(ponctualreceiver_name))
        chart.AddBars(rplvllist)
        ticksBuilder.feed(rplvllist)
        chart.bottom.labels = rplist
    if ticksBuilder.min!=None:
        ticksBuilder.setmargin()
        chart.left.min = ticksBuilder.min
        chart.left.max = ticksBuilder.max
        chart.left.labels = ticksBuilder.GetTicksLabels("%.2f s")
        chart.left.label_positions = ticksBuilder.GetTicksPosition()
        chart.left.label_gridlines = True
        chart.display.extra_params['chf'] = "bg,s,65432100"
        tags_dict["CHART_RT30"]=chart.display.Img(settings.width, settings.height)
    return tags_dict
class ReportBuilder:
    def __init__(self):
        self._dataPr={}
        self._dataRs={}
    def SetPonctualReceiverData(self, prdata):
        self._dataPr=prdata
    def SetSurfaceReceiverData(self, rsdata):
        self._dataRs=rsdata
    def BuildHtmlCode(self, html_code_path, outfolderpath, language):
        #Tags globaux
        tags_dict={
         "MENUBAR_SYNTHESIS" : _("Synthesis"),         
         "MENUBAR_PONCTUALRECEIVER" : _("Punctual receivers"),
         "MENUBAR_STATS" : _("Statistics"),
         "BOTTOM_USECONDITION" : _("Terms of Use"),
         "REPORTLANGUAGE" : language
         }
        #Edition de la liste des libellé des récepteurs ponctuels
        menubar_rslst=""
        prlist=self._dataPr.keys()
        prlist.sort(key=strtodigit)
        for pr_name in prlist:
            menubar_rslst+="""<li><a href="#">%s</a></li>\n""" % (pr_name)
        tags_dict["MENUBAR_RECEIVERMENULIST"]=menubar_rslst
        #Création de la page de synthèse
        pageOneDict=MakePageOne(dict(tags_dict), self._dataPr, self._dataRs)
        #Sauvegarde de la page web
        ReplaceHtmlTags(html_code_path+"raw_webpage.htm",pageOneDict,outfolderpath+"SppsReport.htm")

        #exportation des ressources des pages
        if os.path.exists(outfolderpath+"ressource\\"):
            shutil.rmtree(outfolderpath+"ressource\\")
        shutil.copytree(html_code_path+"ressource\\", outfolderpath+"ressource\\")
