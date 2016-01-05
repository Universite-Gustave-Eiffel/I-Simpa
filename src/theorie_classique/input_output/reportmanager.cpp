#include "reportmanager.h"
#include <input_output/gabe/gabe.h>
using namespace std;

ReportManager::ReportManager(t_ParamReport& _paramReport)
:BaseReportManager()
{
	paramReport=_paramReport;
}


ReportManager::~ReportManager()
{
}
void ReportManager::CopyRecepteurCut(std::vector<r_SurfCut*>& source,std::vector<r_SurfCut*>& destination)
{
	//Copie des récepteurs surfacique
	for(uentier idrs=0;idrs<source.size();idrs++)
	{
		r_SurfCut* newRs=new r_SurfCut(*source[idrs]);
		destination.push_back(newRs);
	}
}
void ReportManager::CopyRecepteurSurf(std::vector<r_Surf*>& source,std::vector<r_Surf*>& destination)
{
	//Copie des récepteurs surfacique
	for(uentier idrs=0;idrs<source.size();idrs++)
	{
		r_Surf* newRs=new r_Surf(*source[idrs]);
		destination.push_back(newRs);
	}
}
void ReportManager::SauveFusionRecepteursSurfaciques(const stringClass& rootFolder,std::vector<r_Surf*>& tabRecepteurS)
{
	using namespace formatGABE;
	for(std::vector<r_Surf*>::iterator rsit=tabRecepteurS.begin();rsit!=tabRecepteurS.end();rsit++)
	{
		if((*rsit)->nbFaces>0)
		{
			int nbfreq=(*rsit)->faces[0].nbfreq;
			GABE rsExporter(nbfreq);
			for(int idfreq=0;idfreq<nbfreq;idfreq++)
			{
				GABE_Data_Float* colrs=new GABE_Data_Float((*rsit)->nbFaces);
				for(uentier idface=0;idface<(*rsit)->nbFaces;idface++)
				{
					colrs->Set(idface,(*rsit)->faces[idface].energieRecu[idfreq][0]);
				}
				rsExporter.SetCol(idfreq,colrs);
			}
			rsExporter.Save((rootFolder+"rs"+CoreString::FromInt((*rsit)->idRecepteurS)+".gabe").c_str());

		}
	}
}
void ReportManager::SauveFusionTCRecepteursPonctuels(const stringClass& folderPath,std::vector<t_TC_RecepteurPonctuel>& tabRecepteurP)
{
	using namespace formatGABE;

	GABE rpExporter(tabRecepteurP.size());
	int numrp=0;
	for(std::vector<t_TC_RecepteurPonctuel>::iterator itrp=tabRecepteurP.begin();itrp!=tabRecepteurP.end();itrp++)
	{
		GABE_Data_Float colrp((*itrp).nbFreq);
		colrp.SetLabel(CoreString::FromInt((*itrp).linkedRecepteurP->index).c_str());
		for(int idfreq=0;idfreq<(*itrp).nbFreq;idfreq++)
			colrp.Set(idfreq,(*itrp).tabDataByFreq[idfreq].ChampDirectLineaire);
		rpExporter.SetCol(numrp,colrp);
		numrp+=1;
	}
	rpExporter.Save(stringClass(folderPath+"rp.gabe").c_str());
}
void ReportManager::SauveTCRecepteursPonctuels(stringClass rootFolder, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& colsLblLst,std::vector<t_TC_RecepteurPonctuel>* tabRecepteurP)
{

	using namespace formatGABE;
	entier nbFreq=freqLst.size();
	entier nbCols=colsLblLst.size();
	//Créé un fichier pour chaque récépteur
	for(std::size_t idrp=0;idrp<tabRecepteurP->size();idrp++)
	{
		t_TC_RecepteurPonctuel* currentRecP=&(*tabRecepteurP)[idrp];

		GABE binExporter(nbCols+1); //+1 pour le libellé des lignes
		binExporter.LockData();
		//Ajout du libellé
		GABE_Data_ShortString* lblCol = new GABE_Data_ShortString(nbFreq+1); // + 1 pour la ligne Global
		for(int idfreq=0;idfreq<nbFreq;idfreq++)
			lblCol->SetString(idfreq,freqLst[idfreq].c_str());
		lblCol->SetString(nbFreq,"Global");
		lblCol->SetLabel("SPL (dB)");
		binExporter.SetCol(0,lblCol);
		//Pour chaque theorie de calcul utilisé
		for(int idcol=0;idcol<nbCols;idcol++)
		{
			//////////////////////////////////
			// Copie des données en dB
			GABE_Data_Float* dataCol = new GABE_Data_Float(nbFreq+1); // + 1 pour la ligne Global
			(*dataCol).SetLabel(colsLblLst[idcol].c_str());
			float globalValue=0.f;
			for(int idfreq=0;idfreq<nbFreq;idfreq++)
			{
				float rowVal;
				if(idcol==0)
					rowVal=currentRecP->tabDataByFreq[idfreq].ChampDirect;
				else
					rowVal=currentRecP->tabDataByFreq[idfreq].tabDataByTheorie[idcol-1].ChampTotal;
				globalValue+=pow(10,rowVal/10.f);
				dataCol->Set(idfreq,rowVal);
			}
			dataCol->Set(nbFreq,10*log10f(globalValue));
			dataCol->headerData.numOfDigits=4;
			binExporter.SetCol(idcol+1,dataCol);
		}
		stringClass fileName=rootFolder+stringClass(currentRecP->linkedRecepteurP->lblRp)+".gabe";
		binExporter.Save(fileName.c_str());
	}

}

void ReportManager::SauveTCGlobalsValues(stringClass filePath, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& colsLblLst,t_mainData& mainData)
{
	using namespace formatGABE;
	entier nbFreq=freqLst.size();
	entier nbCols=colsLblLst.size();
	GABE binExporter(nbCols+1); //+1 pour le libellé des lignes
	binExporter.LockData();

	//Ajout du libellé
	GABE_Data_ShortString* lblCol = new GABE_Data_ShortString(nbFreq+1); // + 1 pour la ligne Global
	for(int idfreq=0;idfreq<nbFreq;idfreq++)
		lblCol->SetString(idfreq,freqLst[idfreq].c_str());
	lblCol->SetString(nbFreq,"Global");
	lblCol->SetLabel("TC");
	binExporter.SetCol(0,lblCol);

	//Pour chaque mode de calcul
	for(int idModeCalcul=0;idModeCalcul<NB_THEORIES;idModeCalcul++)
	{
		//Pour chaque colonne de donnée
		for(entier idData=0;idData<NB_COLS_BY_THEORIE;idData++)
		{
			//////////////////////////////////
			// Copie des données en dB
			GABE_Data_Float* dataCol = new GABE_Data_Float(nbFreq+1); // + 1 pour la ligne Global
			(*dataCol).SetLabel(colsLblLst[(idModeCalcul*NB_COLS_BY_THEORIE)+idData].c_str());
			float globalValue=0.f;

			//Pour chaque bande de fréquence
			for(int idfreq=0;idfreq<nbFreq;idfreq++)
			{
				decimal rowVal;
				switch(idData)
				{
					case 0:
						rowVal=mainData.frequencyDependentValues[idfreq].modeCalcul[idModeCalcul].AireAbsorptionEquivalente;
						break;
					case 1:
						rowVal=mainData.frequencyDependentValues[idfreq].modeCalcul[idModeCalcul].TR;
						(*dataCol).headerData.numOfDigits=4;
						break;
					case 2:
						rowVal=mainData.frequencyDependentValues[idfreq].modeCalcul[idModeCalcul].NiveauSonoreChampReverbere;
						(*dataCol).headerData.numOfDigits=5;
						break;
				};
				globalValue+=pow(10,rowVal/10.f);
				dataCol->Set(idfreq,rowVal);
			}

			if(idData==2)
				dataCol->Set(nbFreq,10*log10f(globalValue));
			else
				dataCol->Set(nbFreq,0);
			binExporter.SetCol((idModeCalcul*NB_COLS_BY_THEORIE)+idData+1,dataCol);
		}
	}

	stringClass fileName=filePath;
	fileName+=".gabe";
	binExporter.Save(fileName.c_str());
}
