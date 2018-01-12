#include <input_output/baseReportManager.h>
#include "../tcTypes.h"

/**
 * @file reportmanager.h
 * @brief Implémentation du gestionnaire de fichiers de rapports
 */

#ifndef __REPORT_MANAGER__
#define __REPORT_MANAGER__

/**
 * @brief Gestionnaire de fichier de sortie à déstination de l'interface PSPS
 *
 * Cette classe permet l'export de tout les fichiers de resultat de calcul.
 */
class ReportManager : public BaseReportManager
{
public:
	/**
	 * @brief Structure décrivant les paramètres d'un objet de rapport 
	 */
	struct t_ParamReport{
		uentier nbTimeStep;
		uentier freqIndex;
		uentier freqValue;
		stringClass _recepteur_surf_Path;
		t_TetraMesh* tetraModel;
		t_Mesh* sceneModel;
	};
private:
	t_ParamReport paramReport;
public:
	/**
	 * Constructeur de la classe
	 *
	 * @param _particlePath Chemin où enregistrer le fichier de particules
	 *
	 */
	ReportManager(t_ParamReport& _paramReport);
	~ReportManager();
	static void CopyRecepteurSurf(std::vector<r_Surf*>& source,std::vector<r_Surf*>& destination); 
	static void CopyRecepteurCut(std::vector<r_SurfCut*>& source,std::vector<r_SurfCut*>& destination);
	static void SauveTCRecepteursPonctuels(stringClass rootFolder, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& colsLblLst,std::vector<t_TC_RecepteurPonctuel>* tabRecepteurP);
	static void SauveTCGlobalsValues(stringClass filePath, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& colsLblLst,t_mainData& mainData);
	static void SauveFusionTCRecepteursPonctuels(const stringClass& folderPath,std::vector<t_TC_RecepteurPonctuel>& tabRecepteurP);
	static void SauveFusionRecepteursSurfaciques(const stringClass& rootFolder,std::vector<r_Surf*>& tabRecepteurS);
	static void SauveFusionCutSurfaceReceiver(const stringClass& rootFolder, std::vector<r_SurfCut*>& tabRecepteurS);
};

#endif