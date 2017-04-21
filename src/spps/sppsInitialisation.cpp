#include "sppsInitialisation.h"
#include <iostream>
inline bool ContainsRP(t_Recepteur_P* recepteurTest, std::vector<t_Recepteur_P*>* rpList)
{
	for(std::size_t idRP=0;idRP<rpList->size();idRP++)
	{
		if(rpList->at(idRP)==recepteurTest)
			return true;
	}
	return false;
}

void TranslateSourceAtTetrahedronVertex(std::vector<t_Source*>& lstSource,t_TetraMesh* tetraMesh)
{
	float distTranslation(0.005f);
	for(std::vector<t_Source*>::iterator itsrc=lstSource.begin();itsrc!=lstSource.end();itsrc++)
	{
		for(unsigned short idvertex=0;idvertex<4;idvertex++)
		{
			if((*itsrc)->Position.distance(tetraMesh->nodes[(*itsrc)->currentVolume->sommets[idvertex]])<BARELY_EPSILON)
			{
				// Move the sound source of distTranslation m to center of tetrahedron
				vec3 centerTetra=GetGTetra(tetraMesh->nodes[(*itsrc)->currentVolume->sommets.a],
					tetraMesh->nodes[(*itsrc)->currentVolume->sommets.b],
					tetraMesh->nodes[(*itsrc)->currentVolume->sommets.c],
					tetraMesh->nodes[(*itsrc)->currentVolume->sommets.d]);
				std::cout<<"Source at tetrahedron vertex, move source position from ["<<(*itsrc)->Position.x<<";"<<(*itsrc)->Position.y<<";"<<(*itsrc)->Position.z<<"]";
				(*itsrc)->Position=(*itsrc)->Position+((centerTetra-(*itsrc)->Position)*distTranslation);
				std::cout<<" to ["<<(*itsrc)->Position.x<<";"<<(*itsrc)->Position.y<<";"<<(*itsrc)->Position.z<<"]"<<std::endl;
				break;
			}
		}
	}
}


/**
 * Méthode récursive de propagation de liaison entre un récepteur ponctuel et les tétraèdres voisins.
 * Pas d'appel récursif si le tétraèdre courant est déjà lié au récepteur ponctuel ou si le récepteur ponctuel ne s'étend pas jusqu'a se volume
 * Si une collision est détecté alors chaque tetraèdre voisin est testé
 * @see ExpandRecepteurPTetraLocalisation
 **/
void RecursiveTetraTest( t_Recepteur_P* recepteurTest, const decimal& rayon, t_Tetra* currentTetra , t_TetraMesh* tetraMesh)
{
	if(!currentTetra)
		return;
	if(currentTetra->linkedRecepteurP)
	{
		if(ContainsRP(recepteurTest,currentTetra->linkedRecepteurP))
		{
			return; // Punctual receiver already linked with sub-volume
		}
	}
	for(int idfaceTri=0;idfaceTri<4;idfaceTri++)
	{
		decimal minLength=sqrtf(ClosestDistanceBetweenDotAndTriangle(
			tetraMesh->nodes[currentTetra->faces[idfaceTri].indiceSommets.a],
			tetraMesh->nodes[currentTetra->faces[idfaceTri].indiceSommets.b],
			tetraMesh->nodes[currentTetra->faces[idfaceTri].indiceSommets.c],
			recepteurTest->position,NULL,NULL));
		if(minLength<=rayon) //If the face is crossed by the sphere
		{
			if(!currentTetra->linkedRecepteurP)
				currentTetra->makeTabRecp();
			currentTetra->linkedRecepteurP->push_back(recepteurTest);
			for(int idVoisin=0;idVoisin<4;idVoisin++)
			{
				RecursiveTetraTest(recepteurTest, rayon, currentTetra->voisins[idVoisin], tetraMesh);
			}
			return;
		}
	}
}

void ExpandPunctualReceiverTetrahedronLocalisation(t_TetraMesh *tetraMesh, std::vector<t_Recepteur_P *> *lstRecepteurP,
												   Core_Configuration &configManager)
{
	decimal rayonRecepteurP=*configManager.FastGetConfigValue(Core_Configuration::FPROP_RAYON_RECEPTEURP);
	double volumeRP=(pow(*configManager.FastGetConfigValue(Core_Configuration::FPROP_RAYON_RECEPTEURP),3)*M_PI*4.)/3.;
	for(std::size_t idrecp=0;idrecp<lstRecepteurP->size();idrecp++)
	{
		if(lstRecepteurP->at(idrecp)->indexTetra<tetraMesh->tetraedresSize)
		{
			t_Tetra* recpTetra=&tetraMesh->tetraedres[lstRecepteurP->at(idrecp)->indexTetra];
			// Compute of the normalisation value of the punctual receiver
			lstRecepteurP->at(idrecp)->cdt_vol=(float)(((*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE))*(*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_RHO)))/volumeRP);
			for(int idVoisin=0;idVoisin<4;idVoisin++)
			{
				RecursiveTetraTest(lstRecepteurP->at(idrecp), rayonRecepteurP, recpTetra->voisins[idVoisin], tetraMesh);
			}
		}
	}
}

bool CheckSourcePosition(std::vector<t_Source *> &lstSource, t_Mesh *model) {
	for (auto &&item : lstSource) {
		for(int idface=0; idface < model->pface_size; idface++) {
			const t_cFace& face = model->pfaces[idface];
			const vec3& a = model->pvertices[face.sommets.a];
			const vec3& b = model->pvertices[face.sommets.b];
			const vec3& c = model->pvertices[face.sommets.c];
			decimal dist;
			// Compute distance from this 3D model face
			// If distance is too close then computation will be canceled
			if(DotIsInVertex(item->Position, a, b, c, &dist) && dist < BARELY_EPSILON) {
				return false;
			}
		}
	}
	return true;
}

