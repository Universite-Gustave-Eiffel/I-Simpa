#include <coreTypes.h>
#include "data_manager/core_configuration.h"

/**
 * En fonction du rayon des récepteurs ponctuels, les tétraèdres voisin seront liés à celui-ci
 * Réafecte t_Recepteur_P::cdt_vol à (Celerite*Rho)/(volume sphere recepteur Ponctuel)
 */
void ExpandPunctualReceiverTetrahedronLocalisation(t_TetraMesh *tetraMesh, std::vector<t_Recepteur_P *> *lstRecepteurP,
                                                   Core_Configuration &configManager);
/**
 * Slighty move source that are too near from a tetrahedron vertex
 */
void TranslateSourceAtTetrahedronVertex(std::vector<t_Source*>& lstSource,t_TetraMesh* tetraMesh);

/**
 * Check if there is sound source in collision with 3d model
 * @param lstSource List of sound sources
 * @param model 3D model
 * @return True if sound source position is ok, false otherwise
 */
bool CheckSourcePosition(std::vector<t_Source*>& lstSource, t_Mesh* model);