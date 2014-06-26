def lsttocomsoldict(lst):
    retv=""
    for val in lst:
        retv+=str(val)+","
    return "{"+retv[:-1]+"}"
def lsttocomsollst(lst):
    retv=""
    for val in lst:
        retv+=str(val)+","
    return "["+retv[:-1]+"]"
def GetApplFromPosition( pos, tetramesh, splitter, appl_lst, domains_lst, domains_dict ):
    """
    Retourne la variable d'application u(n) en fonction d'une position dans l'espace
    """
    if len(appl_lst)==1:
        return "'u'"
    idtet=splitter.GetTetraByPosition( tetramesh, pos[0],pos[1],pos[2])
    domain_container_xmlid=tetramesh.tetrahedres[idtet].idVolume
    domain_container_locid=domains_dict[domain_container_xmlid]
    return "'"+appl_lst[domains_lst[domain_container_locid].GetApplicationUsage()-1].varapp+"'"