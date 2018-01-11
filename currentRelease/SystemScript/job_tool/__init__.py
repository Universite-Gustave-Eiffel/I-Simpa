# -*- coding: UTF-8 -*-
import uictrl as ui
import operator
import uilocale
import os

_=uilocale.InstallUiModule(ui.application.getapplicationpath()["systemscript"]+"job_tool"+os.sep,ui.application.getlocale())

def GetCurrentProjectPath():
    projconfel=ui.element(ui.element(ui.application.getrootscene()).getelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION))
    return projconfel.getstringconfig("urlsave")
job_types={}

class job(object):
    """
        Cette classe représente un travail
        Elle peut être importée et exportée sous forme de chaînes de caractères
    """
    def __init__(self,params):
        self.jobname=params[0]
        self.from_str(params)
    def __init__(self,jobname,idel):
        self.jobname=jobname
        self._jobinit_(idel)
    def _jobinit_(self,idel):
        pass
    def to_str(self):
        return self.jobname+" "
    def Execute(self):
        pass
    def from_str(self,params):
        """
        Initialisation à partir d'un tableau de chaînes params
        """
        pass

class job_run(job):
    """
    Exécution d'un code de calcul
    """
    def _jobinit_(self,idel):
        self.typecore=ui.element(idel).getinfos()["name"]
    def to_str(self):
        return job.to_str(self)+self.typecore
    def Execute(self):
        rootcore=ui.element(ui.application.getrootcore())
        core=ui.element(rootcore.getelementbylibelle(self.typecore))
        ui.application.sendevent(core,ui.idevent.IDEVENT_RUN_CALCULATION)
    def from_str(self,params):
        """
        Initialisation à partir d'un tableau de chaînes params
        """
        self.typecore=params[1]
job_types["RUN"]=job_run

class job_openproject(job):
    """
    Ouverture d'un projet
    """
    def _jobinit_(self,idel):
        self.path=GetCurrentProjectPath()
    def to_str(self):
        return job.to_str(self)+self.path
    def Execute(self):
        if self.path!=GetCurrentProjectPath():
            ui.application.loadproject(self.path)
    def from_str(self,params):
        """
        Initialisation à partir d'un tableau de chaînes params
        """
        fullpath=""
        for piece in params[1:]:
            fullpath+=piece+" "
        fullpath=fullpath[:-1]
        self.path=fullpath
job_types["OPEN_PROJECT"]=job_openproject

class job_saveproject(job):
    """
    Ouverture d'un projet
    """
    def _jobinit_(self,idel):
        pass
    def to_str(self):
        return job.to_str(self)
    def Execute(self):
        ui.application.saveproject()
    def from_str(self,params):
        """
        Initialisation à partir d'un tableau de chaînes params
        """
        pass
job_types["SAVE_PROJECT"]=job_saveproject

class JobManager(object):
    """
        Cette classe permet la gestion des travaux renseignés par l'utilisateur
    """
    def appendjob(self,jobname,identifier):
        if operator.isSequenceType(identifier):
            self.joblst.append(job_types[jobname](identifier))
        else:
            self.joblst.append(job_types[jobname](jobname,identifier))
    def __init__(self):
        self.joblst=[]
        self.lastprojectpath=""
    def ExecJobs(self,idel):
        for job in self.joblst:
            job.Execute()
    def AddOpenProjectIfChanges(self):
        curpath=GetCurrentProjectPath()
        if curpath!=self.lastprojectpath:
            self.appendjob("OPEN_PROJECT",0)
            self.lastprojectpath=curpath
    def AddRun(self,idel):
        self.AddOpenProjectIfChanges()
        self.appendjob("RUN",idel)
        self.appendjob("SAVE_PROJECT",idel)
    def ClearJobLst(self,idel):
        self.joblst=[]
        self.lastprojectpath=""
    def ImportJobLst(self,idel):
        pass
    def ExportJobLst(self,idel):
        pass
    def PrintJobLst(self,idel):
        print _(u"%i tasks:" % (len(self.joblst)))
        for job in self.joblst:
            print job.to_str()

class manager:
    def __init__(self):
        self.jmanager=JobManager()
        self.addcalculation_id=ui.application.register_event(self.jmanager.AddRun)
        self.clear_jobs_id=ui.application.register_event(self.jmanager.ClearJobLst)
        self.execute_jobs_id=ui.application.register_event(self.jmanager.ExecJobs)
        self.print_jobs_id=ui.application.register_event(self.jmanager.PrintJobLst)
    def getmenu(self,typeel,idel,menu):
        submenu=[]
        submenu.append((_(u"Add calculation in the job list"),self.addcalculation_id))
        submenu.append(())
        submenu.append((_(u"Clear job list"),[(_(u"Confirmation"),self.clear_jobs_id)]))
        submenu.append((_(u"Run job list"),self.execute_jobs_id))
        submenu.append((_(u"Show job list"),self.print_jobs_id))
        menu.insert(2,())
        menu.insert(2,(_(u"Job list"),submenu))
        return True
menu_manager=manager()
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_CORE_SPPS,menu_manager )
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_CORE_TC,menu_manager )
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_CORE_CORE,menu_manager )
