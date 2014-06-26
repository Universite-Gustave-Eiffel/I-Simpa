# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import exceptions
from libsimpa import *
import sys
import os
import make_recsurf_ar
from meshElement import meshElement
from fittingElement import fittingElement
from tools import *
from make_recp_ar import GetPonctualReceiverDeclaration
import com_app

##
# @file makecomscript.py
# \~english 
# Build comsol script from core input files.


##
# \~english 
# @param a Vertice index of the first triangle vertice
# @param b Vertice index of the second triangle vertice
# @param c Vertice index of the third triangle vertice
# @param vertices List, Vertices coordinates
# @return Tuple, (Ax,Bx,Cx,Ay,By,Cy,Az,Bz,Cz) 
def getfacecoords(a,b,c,vertices):
    va=vertices[a]
    vb=vertices[b]
    vc=vertices[c]
    return (va.x,vb.x,vc.x,va.y,vb.y,vc.y,va.z,vb.z,vc.z)

##
# \~english 
# @param coreconf coreConfig instance
# @param appl_lst Application list
# @return Comsol script of the solver call
def getsolver_func(coreconf,appl_lst):
    appl_decl=[]
    for appl in appl_lst:
        appl_decl.append("'"+appl.varapp+"'")
    appl_decl=lsttocomsoldict(appl_decl)
    if coreconf.const["temporel"]:
        return """femtime(fem, ...
                    'solcomp',%s, ...
                    'outcomp',%s, ...
                    'tlist',[0:%f:%f], ...
                    'tout','tlist', ...
                    'linsolver','gmres');""" % (appl_decl,appl_decl,coreconf["simulation"].getpropertyfloat("pasdetemps"),coreconf["simulation"].getpropertyfloat("duree_simulation"))
    else:
        return """femstatic(fem, ...
                      'solcomp',%s, ...
                      'outcomp',%s, ...
                      'ntol',1e-006);""" % (appl_decl,appl_decl)


"""
Création du code ComSol afin de calculer les paramètres définis par PSPS
"""
##
# \~english 
# @return The comsol script declaration of the rec_surf struct.
# @param rsurf_dict Dictionnary with rsurf xml id as key and rsurf instance as value.
# @param coreconf coreConfig instance
def ConstructSceneRSurfLst(rsurf_dict,coreconf):
    out_m_code="rec_surf.faces={};\n"
    out_m_code+="rec_surf.appl={};\n"
    countsurf=1
    for k,v in rsurf_dict.iteritems():
        strfilelist=str(v.facelst).replace("], [","] [").replace(",",";")
        out_m_code+="rec_surf.faces{%i}=%s;\n" % (countsurf,strfilelist)
        out_m_code+="rec_surf.appl{%i}=%s;\n" % (countsurf,lsttocomsoldict(v.appname))
        countsurf+=1
    out_m_code+="rec_surf.names=%s;\n" % (str(rsurf_dict.keys()))
    freqlabels=[]
    coreconf.freqlbl=[]
    for freq in coreconf.freqlst:
        freqlabels.append("'%s Hz\\'" % (str(freq)))
        coreconf.freqlbl.append("%s Hz\\" % (str(freq)))
    out_m_code+="rec_surf.freqlbl=%s;\n" % (lsttocomsoldict(freqlabels))
    out_m_code+="rec_surf.folderrsurf='%s';\n" % (coreconf.paths["recepteurss_directory"])
    return out_m_code
##
# \~english 
# Make surface receivers folder in the output directory folder.
# @param coreconf coreConfig instance
def MakeRecepteurSurfFolders(coreconf):
    if len(coreconf.recepteurssurf)>0:
        rootpath=coreconf.paths["workingdirectory"]+coreconf.paths["recepteurss_directory"]
        for idfreq in coreconf.freqid_docalclst:
            pathtocreate=rootpath+coreconf.freqlbl[idfreq-1]
            if not os.path.exists(pathtocreate):
                os.makedirs(pathtocreate)
##
# \~english 
# Make ponctual receivers folder in the output directory folder.
# @param coreconf coreConfig instance
def MakeRecepteurPonctFolders(coreconf):
    rootpath=coreconf.paths["workingdirectory"]+coreconf.paths["recepteursp_directory"]+"\\"
    for rpnode in coreconf.rootnode["recepteursp"].lstnodesenum("recepteur_ponctuel"):
        pathtocreate=rootpath+rpnode.getproperty("lbl")
        if not os.path.exists(pathtocreate):
            os.makedirs(pathtocreate)

##
# \~english 
# @param coreconf coreConfig instance
# Split the tetrahedral mesh in several Sub-Domains
# @param[in-out] domains_lst List of domainElement instances (inherited class).
# @param[in-out] domains_dict Dictionnary, with domain xml id as key and domain comsol id (list id) as value.
# @param scene 3d model
# @param mesh Tetrahedral mesh
# @param coreconf Core configuration instance (coreConfig)
# @param polygons List of polygons (triangles fusions)
def MakeVolsMesh(domains_lst,domains_dict,scene,mesh,coreconf,polygons):
    """
        Déclaration des volumes
        
    """
    splitter=VolumesSplitter()
    coreconf.splitter=splitter
    
    splitter.LoadDomain(scene,mesh)
    sceneLoader=CformatBIN()
    for ivol in range(0,len(splitter)):
        domainClass=meshElement
        volinfos=splitter[ivol]
        idvol=volinfos[0]
        #Si ce domaine est de type encombrement on change de classe
        if idvol in coreconf.fittings.keys():
            domainClass=fittingElement
        newvol=domainClass("dom%i"%(idvol),None,coreconf)
        newvol.InitFromSplitterData(volinfos,scene,coreconf,sceneLoader,polygons)
        domains_lst.append(newvol)
    
    for iddomaine,domain in enumerate(domains_lst):
        domains_dict[domain.volid]=iddomaine
    for domain in domains_lst:
        domain.PostCreationComputation(domains_lst,domains_dict,coreconf) 
##
# Generate the comsol script from project data
# @param coreconf coreConf.coreConf instance
# @param rawcomsolscriptfolder Folder of the MD core. Usually "core/md/"
# @return comscript,rec_surf_lst,mesh,sommets,appl_lst
def BuildComsolScript( coreconf, rawcomsolscriptfolder ):
    outputdir=coreconf.paths["workingdirectory"]
    scenemeshpath=outputdir+coreconf.paths["modelName"]
    meshpath=outputdir+coreconf.paths["tetrameshFileName"]
    script_rawfile="finalscript_raw.m"

    #Création des objets scene et le lecteur de fichier de scene
    scene=ioModel()
    sceneLoader=CformatBIN()
    mesh_reader=CMBIN()
    #lecture de la scène
    print("Loading %s" %(scenemeshpath))
    if sceneLoader.ImportBIN(scene,str(scenemeshpath)):
        print("Chargement de la scène effectué")
        #Lecture du maillage
        mesh=mesh_reader.LoadMesh(str(meshpath))
        print("Chargement du maillage effectué")
        coreconf.tetmesh=mesh
        coreconf.scene=scene
        VOL_SCENE=mesh_reader.ComputeVolume(mesh)
        comscript="finalscript_gen"
        outputfolder=outputdir+comscript+".m"
        #chargement des recepteurs surfacique
        rec_surf_lst={}
        sommets=[]
        inputscript=open(rawcomsolscriptfolder+script_rawfile,'r')
        outputscript=open(outputfolder,'w')
        print("Ouverture du fichier matlab.")
        MODELFACES=""
        MODELVERTICES=""
        mat=[]
        iface=0
        SURF_SCENE=0
        domains_lst=[]
        domains_dict={}
        #Construction des groupes de surfaces coplanaires
        merge_tool=SurfaceMerging()
        merge_tool.LoadGroups(scene)
        print("Groupes de surfaces chargés.")
        polygons=[]
        for vertice in scene.vertices:
            MODELVERTICES+="%f,%f,%f;\n" % (vertice.x,vertice.y,vertice.z)
        for idpoly in range(0,merge_tool.GetGroups()):
            lsttri=list(merge_tool.GetGroupInformation(idpoly))
            polygons.append(lsttri)
            v_matrix=""
            for idface in lsttri:
                v_matrix+=";%i,%i,%i" % (scene.faces[idface].a+1,scene.faces[idface].b+1,scene.faces[idface].c+1)
            v_matrix="["+v_matrix[1:]+"]"
            MODELFACES+="grp_face_to_poly(verts,%s)\n" % (v_matrix)
            

        for face in scene.faces:
            #MODELFACES+="triface(vertices(%i,:),vertices(%i,:),vertices(%i,:))\n" % (face.c+1,face.b+1,face.a+1)
            #MODELFACES+="face3([%f;%f;%f],[%f;%f;%f],[%f;%f;%f])\n" % getfacecoords(face.a,face.b,face.c,scene.vertices)
            #aire=sceneLoader.GetSurfAire(scene,iface)
            #scene.faces[iface].aire=aire
            SURF_SCENE+=sceneLoader.GetSurfAire(scene,iface)
            #mat.append(coreconf.GetMaterialComsolIndiceByXmlMaterialIndice(face.idMat))
            iface+=1
        domains_lst+=coreconf.sources_lst
        MakeVolsMesh(domains_lst,domains_dict,scene,mesh,coreconf,polygons)
        #domains_lst[0].SetMaterials(mat)
        #
        # Construction du code comsol pour les éléments de domaines (Salles,Encombrement et sources)
        DOMAIN_DECLARATIONS=""
        DOMAIN_ARRAY=[]        
        DOMAIN_NAME_ARRAY=[]
        DOMAIN_NAME_TAGS=[]
        for dom in domains_lst:
            DOMAIN_DECLARATIONS+="%s\n" % (dom.GetComsolElementDeclaration())
            DOMAIN_ARRAY.append(dom.GetDomainName())
            DOMAIN_NAME_ARRAY.append(dom.GetDomainParam("names"))
            DOMAIN_NAME_TAGS.append("'%s'" % (dom.GetDomainName()))
        DOMAIN_ARRAY=lsttocomsoldict(DOMAIN_ARRAY)    
        DOMAIN_NAME_ARRAY=lsttocomsoldict(DOMAIN_NAME_ARRAY)
        DOMAIN_NAME_TAGS=lsttocomsoldict(DOMAIN_NAME_TAGS)
        # Déclaration des matériaux
        #
        MATERIALS_DECLARATION=""
        matprops=["q","name","h","type","g"]
        for prop in matprops:
            proplst=[]
            for mat in coreconf.materials:
                proplst.append(mat.get(prop,0)) #lit la propriété prop, ou affecte 0 si inexistant
            MATERIALS_DECLARATION+="mat.materials.%s=%s;\n" % (prop,lsttocomsoldict(proplst))
        volmatprops=["assoc","faces"]
        for prop in volmatprops:
            proplst=[]
            for dom in domains_lst:
                proplst.append(dom.GetMaterialParam(prop))
            MATERIALS_DECLARATION+="mat.%s=%s;\n" % (prop,lsttocomsoldict(proplst))
        # Définition des équations des domaines
        #
        VOLUMES_DECLARATION=""
        volprops=["names","f","a","da","c","init"]
        for prop in volprops:
            proplst=[]
            for dom in domains_lst:
                proplst.append(dom.GetDomainParam(prop))
            VOLUMES_DECLARATION+="vol.%s= %s;\n" % (prop,lsttocomsoldict(proplst))
        ##########################################
        # Equations dépendantes de la fréquences
        #               [ [ in, out ],
        freq_equations=[["mat.materials.q","bnd.q"],
                        ["vol.f","equ.f"],
                        ["vol.a","equ.a"],
                        ["vol.c","equ.c"],
                        ["vol.init","equ.init"],
                        ["fem.appl{ID_APP}.bnd.gallfreq","bnd.g"]
                        ]

        VOLUMES_DECLARATION+="vol.assoc=%s;" % (lsttocomsoldict(range(1,len(domains_lst)+1)))
        APPL_DECLARATION=""
        EQUATIONS_FREQUENCY_TRANSLATION=""
        appl_lst=com_app.MakeApps(domains_lst,scene,mesh,coreconf,polygons)
        for idapp,app in enumerate(appl_lst):
            APPL_DECLARATION+=app.GetDeclaration(coreconf,domains_lst, domains_dict,appl_lst)
            for equ in freq_equations:
                allfreq_src=equ[0]
                if "ID_APP" in allfreq_src:
                    allfreq_src=allfreq_src.replace("ID_APP",str(idapp+1))
                EQUATIONS_FREQUENCY_TRANSLATION+=chr(9)+"fem.appl{%i}.%s=ChangeFreq(%s,fem.appl{%i}.%s,idfreq);\n" % (idapp+1,equ[1],allfreq_src,idapp+1,equ[1])
       
        if len(coreconf.recepteurssurf)>0:
            print("Construction de la liste des récepteurs de surfaces")
            rec_surf_lst,sommets=make_recsurf_ar.GetRecepteurSurfList(scene,mesh,coreconf,appl_lst,domains_lst, domains_dict)
            print("Construction terminée")
        # ################################################################################
        # Lecture du fichier modèle et remplacement des mots clées par les scripts générés
        #
        #
        keyword_replacement={}
        keyword_replacement["MODELFACES"]=MODELFACES
        keyword_replacement["R_SURF_DECLARATION"]=ConstructSceneRSurfLst(rec_surf_lst,coreconf)
        keyword_replacement["VOL_SCENE"]=str(VOL_SCENE)
        keyword_replacement["SURF_SCENE"]=str(SURF_SCENE)
        keyword_replacement["CELERITE_SON"]=str(coreconf.const["cel"])
        keyword_replacement["TIME_STEP"]=str(coreconf.const["pasdetemps"])
        keyword_replacement["DOMAIN_DECLARATIONS"]=DOMAIN_DECLARATIONS
        keyword_replacement["DOMAIN_ARRAY"]=DOMAIN_ARRAY
        keyword_replacement["DOMAIN_NAME_ARRAY"]=DOMAIN_NAME_ARRAY
        keyword_replacement["DOMAIN_NAME_TAGS"]=DOMAIN_NAME_TAGS
        keyword_replacement["MATERIALS_DECLARATION"]=MATERIALS_DECLARATION
        keyword_replacement["VOLUMES_DECLARATION"]=VOLUMES_DECLARATION
        keyword_replacement["FREQLST"]=lsttocomsollst(coreconf.freqid_docalclst)
        keyword_replacement["EQUATIONS_FREQUENCY_TRANSLATION"]=EQUATIONS_FREQUENCY_TRANSLATION
        keyword_replacement["R_PONCT_DECLARATION"]=GetPonctualReceiverDeclaration(coreconf,appl_lst,domains_lst, domains_dict)
        keyword_replacement["SOLVER_FUNCTION"]=getsolver_func(coreconf,appl_lst)
        keyword_replacement["MODELVERTICES"]=MODELVERTICES
        keyword_replacement["APPL_DECLARATION"]=APPL_DECLARATION
        print("Création du fichier finalscript_gen.m")
        for line in inputscript:
            for k,v in keyword_replacement.iteritems():
                if k in line:
                    line=line.replace(k,v)
                    break
            outputscript.write(line)
        print("Sauvegarde du script vers %s" % (outputfolder))
        inputscript.close()
        outputscript.close()

        print("Création des dossiers de sorties")
        MakeRecepteurSurfFolders(coreconf)
        MakeRecepteurPonctFolders(coreconf)
        print("Fin de l'opération")
        return comscript,rec_surf_lst,mesh,sommets,appl_lst
    else:
        mess="Unable to load scenemesh file %s" % (scenemeshpath)
        raise exceptions.Exception(mess)
        return ""
