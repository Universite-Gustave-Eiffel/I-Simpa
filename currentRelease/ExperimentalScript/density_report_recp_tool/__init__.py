# -*- coding: cp1252 -*-

import uictrl as ui
from libsimpa import *
import os
import xml.etree.ElementTree as ET

rho = 1.204
cel = 343.8
p0 = ((20 * 10 ** -6) ** 2)


def extractReceiverPositionFromXml(xmlFilePath):
    tree = ET.parse(xmlFilePath)
    root = tree.getroot()

    receivers_data = []

    def toNum(val):
        return float(val.replace(",", "."))

    for receiver_group in root.iter('recepteursp'):
        for receiver in receiver_group.iter('recepteur_ponctuel'):
            receivers_data.append([receiver.attrib["name"], toNum(receiver.attrib["x"]), toNum(receiver.attrib["y"]),
                                   toNum(receiver.attrib["z"])])

    # sort receiver by x coordinate

    receivers_data.sort(key=lambda item: item[1])

    return receivers_data


# Fonction permettant de calculer la densité d'énergie à partir du niveau de pression
def GetMixedLevel(folderwxid):
    """
     Retourne un tableau contenant le niveau sonore global et toute bande des récepteurs ponctuels d'un dossier
     folderwxid identifiant wxid de l'élément dossier contenant les récepteurs ponctuels.
    """
    i_cols = []  # Initialisation
    myfrequency_labels = []  # Initialisation
    # folder devient l'objet dossier
    recplist = []  # Initialisation
    folder = ui.element(folderwxid)
    # retrieve computation result main folder location
    folderpath = ui.e_file(folder.getinfos()["parentid"]).buildfullpath()
    recpPosition = extractReceiverPositionFromXml(os.path.join(folderpath, 'config.xml'))
    for child in folder.childs():
        if child[1] == ui.element_type.ELEMENT_TYPE_REPORT_FOLDER:
            recplist.append(child[0])  # On incrémente le vecteur "recplist" avec le nombre de récepteurs
    # Pour chaque récepteur on demande a l'application les données traitées du fichier ( niveau sonore et cumuls )

    splElements = {}
    for idrecp in recplist:  # On regarde pour un récepteur donné
        # recp devient l'objet ayant comme indice idrecp (entier)
        parent = ui.element(idrecp)  # On sélectionne la ligne correspondantau récepteur "Ri"
        # application.sendevent(pere,idevent.IDEVENT_RELOAD_FOLDER)
        receiverName = parent.getinfos()[u"label"]  # Dans "receiverName" nous plaçons le nom du récepteur "Ri"
        # on recupere les données calculées
        idel = parent.getelementbylibelle("Sound level")
        if idel != -1:
            splElement = ui.element(idel)
            splElements[receiverName] = splElement
    for receiverInfo in recpPosition:
        receiverName, rx, ry, rz = receiverInfo
        if receiverName in splElements:
            splElement = splElements[receiverName]
            # on stocke dans gridspl le tableau des niveaux de pression
            fullsplgrid = zip(*ui.application.getdataarray(splElement))
            splgrid = fullsplgrid[-1][1:-1]
            # retrieve the last row
            if len(myfrequency_labels) == 0:
                myfrequency_labels = list(fullsplgrid[0][1:-1])
                myfrequency_labels.insert(0, "z")
                myfrequency_labels.insert(0, "y")
                myfrequency_labels.insert(0, "x")
                myfrequency_labels.insert(0, "")
            myWRow = [receiverName,rx,ry,rz]
            for idfreq in range(len(splgrid)):
                w = (p0 * 10 ** (splgrid[idfreq] / 10.)) / (
                rho * cel ** 2)  # Calcul de la densité pour chaque bande de fréquence
                myWRow.append(w)  # Alimentation d'un vecteur densité
            i_cols.append(myWRow)
    i_cols.insert(0, myfrequency_labels)  # Insertion de la fréquence
    return i_cols


# Fonction permettant de lire et d'écrire des fichiers Gabe    
def SaveLevel(tab, path):
    # Creation de l'objet qui lit et ecris les fichier gabe
    gabewriter = Gabe_rw(len(tab))
    labelcol = stringarray()
    for cell in tab[0][1:]:
        labelcol.append(cell.encode('cp1252'))
    gabewriter.AppendStrCol(labelcol, "label")
    for col in tab[1:]:
        datacol = floatarray()
        for cell in col[1:]:
            datacol.append(float(cell))
        gabewriter.AppendFloatCol(datacol, str(col[0]))
    gabewriter.Save(path.encode('cp1252'))


# Fonction permettant de créer des fichiers Gabe avec les densités d'énergie
def dofusion(folderwxid, path):
    arraydata = GetMixedLevel(folderwxid)
    SaveLevel(zip(*arraydata), path)
    # raffraichie l'arbre complet
    ui.application.sendevent(ui.element(ui.element(ui.application.getrootreport()).childs()[0][0]),
                             ui.idevent.IDEVENT_RELOAD_FOLDER)


class manager:
    def __init__(self):
        self.GetD = ui.application.register_event(self.OnDFusion)

    def getmenu(self, typeel, idel, menu):
        el = ui.element(idel)
        infos = el.getinfos()
        if infos["name"] == u"Punctual receivers":
            menu.insert(0, ())
            menu.insert(0, ("Compute Energy Density", self.GetD))
            return True
        else:
            return False

    def OnDFusion(self, idel):
        grp = ui.e_file(idel)
        dofusion(idel, grp.buildfullpath() + ui._("TousDens") + ".gabe")


ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_REPORT_FOLDER, manager())
