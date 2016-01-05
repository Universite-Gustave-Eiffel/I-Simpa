#include <coretypes.h>
#include "data_manager/core_configuration.h"

/**
 * En fonction du rayon des récepteurs ponctuels, les tétraèdres voisin seront liés à celui-ci
 * Réafecte t_Recepteur_P::cdt_vol à (Celerite*Rho)/(volume sphere recepteur Ponctuel)
 */
void ExpandRecepteurPTetraLocalisation(t_TetraMesh* tetraMesh,std::vector<t_Recepteur_P*>* lstRecepteurP,Core_Configuration& configManager);
/**
 * Effectue une translation de distTranslation des sources positionnées sur le sommet d'un tédraèdre
 */
void TranslateSourceAtTetrahedronVertex(std::vector<t_Source*>& lstSource,t_TetraMesh* tetraMesh);