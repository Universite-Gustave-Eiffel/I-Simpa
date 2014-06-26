#include "rsbin.h"
#include "std_rsbin.hpp"
#include <string>
#include "Core/mathlib.h"
#include <limits>


namespace formatRSBIN
{
#define FINDMINMAX_RSURF(x0,x1,x2,min,max) \
	  if(x1<min) min=x1;\
	  if(x1>max) max=x1;\
	  if(x2<min) min=x2;\
	  if(x2>max) max=x2;

#ifdef IS_VAL_BETWEEN
#undef IS_VAL_BETWEEN
#endif
#define IS_VAL_BETWEEN(isolvl,Av,Bv) (((isolvl > Av) && (isolvl <  Bv)) || (isolvl < Av) && (isolvl >  Bv))
#define IS_OUT_OF_ISO(isolvl,v0,v1,v2) (((isolvl < v0) && (isolvl < v1) && (isolvl < v2)) || ((isolvl > v0) && (isolvl > v1) && (isolvl > v2)))

	rsurf_data::rsurf_data()
	{
		this->localData=new t_ExchangeData();
		manageDelete=true;
	}
	rsurf_data::~rsurf_data()
	{
		if(manageDelete)
			delete this->localData;
	}
	void rsurf_data::SetRsBinData(t_ExchangeData* rsData)
	{
		if(manageDelete)
			delete this->localData;
		this->localData=rsData;
		manageDelete=true;
	}
	void rsurf_data::SetRsBinData(t_ExchangeData& rsData)
	{
		if(manageDelete)
			delete this->localData;
		this->localData=&rsData;
		manageDelete=false;
	}
	std::size_t rsurf_data::GetRsCount()
	{
		return this->localData->tabRsSize;
	}
	void rsurf_data::Make(const std::size_t& tabNodesSize,const std::size_t& tabRsSize,const int& nbtimestep,const float& timestep,const std::string& record_type)
	{
		delete[] this->localData->tabNodes;
		delete[] this->localData->tabRs;
		this->localData->tabNodes=new t_nodesPosition[tabNodesSize];
		this->localData->tabRs=new t_ExchangeData_Recepteurs[tabRsSize];
		this->localData->nbTimeStep=nbtimestep;
		this->localData->tabNodesSize=tabNodesSize;
		this->localData->tabRsSize=tabRsSize;
		this->localData->timeStep=timestep;
		if(record_type=="SPL_STANDART")
			this->localData->recordType=RECEPTEURS_RECORD_TYPE_SPL_STANDART;
		else if(record_type=="SPL_GAIN")
			this->localData->recordType=RECEPTEURS_RECORD_TYPE_SPL_GAIN;
		else if(record_type=="TR")
			this->localData->recordType=RECEPTEURS_RECORD_TYPE_TR;
		else if(record_type=="EDT")
			this->localData->recordType=RECEPTEURS_RECORD_TYPE_EDT;
	}

	void rsurf_data::CheckRsParameter(const std::size_t& rsIndex) const
	{
		if((int)rsIndex>=this->localData->tabRsSize)
			throw "Wrong index rsIndex parameter";
	}
	void rsurf_data::CheckFaceParameter(const std::size_t& rsIndex,const std::size_t& faceIndex) const
	{
		CheckRsParameter(rsIndex);
		if((int)faceIndex>=this->localData->tabRs[rsIndex].dataRec.quantFaces)
			throw "Wrong index faceIndex parameter";
	}
	void rsurf_data::CheckNodeParameter(const std::size_t& nodeIndex) const
	{
		if((int)nodeIndex>=this->localData->tabNodesSize)
			throw "Wrong index nodeIndex parameter";
	}

	void rsurf_data::CheckRecordParameter(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& recordIndex) const
	{
		CheckFaceParameter(rsIndex,faceIndex);
		if((int)recordIndex>=this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.nbRecords)
			throw "Wrong index recordIndex parameter";
	}
	void rsurf_data::MakeRs(const std::size_t& rsIndex, const std::size_t& nbfacesconst, const std::string& rs_name, const int& xmlid)
	{
		CheckRsParameter(rsIndex);
		delete[] this->localData->tabRs[rsIndex].dataFaces;
		this->localData->tabRs[rsIndex].dataFaces=new t_ExchangeData_Face[nbfacesconst];
		this->localData->tabRs[rsIndex].dataRec.quantFaces=nbfacesconst;
		this->localData->tabRs[rsIndex].dataRec.xmlIndex=xmlid;
		//strcpy_s(this->localData->tabRs[rsIndex].dataRec.recepteurSName,rs_name.size(),rs_name.c_str());
		strcpy(this->localData->tabRs[rsIndex].dataRec.recepteurSName,rs_name.c_str());
	}
	void rsurf_data::SetNodeValue(const std::size_t& nodeIndex, const float& x, const float& y, const float& z)
	{
		CheckNodeParameter(nodeIndex);
		this->localData->tabNodes[nodeIndex].node[0]=x;
		this->localData->tabNodes[nodeIndex].node[1]=y;
		this->localData->tabNodes[nodeIndex].node[2]=z;
	}

	void rsurf_data::SetFaceInfo(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& vertexA,const std::size_t& vertexB,const std::size_t& vertexC,const std::size_t& recordCount)
	{
		CheckFaceParameter(rsIndex,faceIndex);
		delete[] this->localData->tabRs[rsIndex].dataFaces[faceIndex].tabTimeStep;
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].tabTimeStep=new t_faceValue[recordCount];
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.nbRecords=recordCount;
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex[0]=vertexA;
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex[1]=vertexB;
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex[2]=vertexC;
	}

	void rsurf_data::SetFaceEnergy(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& recordIndex,const std::size_t& idstep,const float& energy)
	{
		CheckRecordParameter(rsIndex,faceIndex,recordIndex);
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].tabTimeStep[recordIndex].energy=energy;
		this->localData->tabRs[rsIndex].dataFaces[faceIndex].tabTimeStep[recordIndex].timeStep=idstep;
	}

	void rsurf_data::GetFileInfos(std::size_t& rs_size,std::size_t& nodes_size,std::size_t& nbtimestep,float& timestep,std::string& record_type) const
	{
		rs_size=this->localData->tabRsSize;
		nodes_size=this->localData->tabNodesSize;
		nbtimestep=this->localData->nbTimeStep;
		timestep=this->localData->timeStep;
		if(this->localData->recordType==RECEPTEURS_RECORD_TYPE_SPL_STANDART)
			record_type="SPL_STANDART";
		else if(this->localData->recordType==RECEPTEURS_RECORD_TYPE_SPL_GAIN)
			record_type="SPL_GAIN";
		else if(this->localData->recordType==RECEPTEURS_RECORD_TYPE_TR)
			record_type="TR";
		else if(this->localData->recordType==RECEPTEURS_RECORD_TYPE_EDT)
			record_type="EDT";
	}

	void rsurf_data::GetNodePositionValue(const std::size_t& nodeIndex, float& x, float& y, float& z)  const
	{
		CheckNodeParameter(nodeIndex);
		x=this->localData->tabNodes[nodeIndex].node[0];
		y=this->localData->tabNodes[nodeIndex].node[1];
		z=this->localData->tabNodes[nodeIndex].node[2];
	}

	void rsurf_data::GetRsInfo(const std::size_t& rsIndex, std::size_t& nbfaces, std::string& rs_name, int& xmlid) const
	{
		CheckRsParameter(rsIndex);
		nbfaces=this->localData->tabRs[rsIndex].dataRec.quantFaces;
		rs_name=std::string(this->localData->tabRs[rsIndex].dataRec.recepteurSName);
		xmlid=this->localData->tabRs[rsIndex].dataRec.xmlIndex;
	}

	void rsurf_data::GetFaceInfo(const std::size_t& rsIndex,const std::size_t& faceIndex,std::size_t& vertexA,std::size_t& vertexB,std::size_t& vertexC,std::size_t& recordCount) const
	{
		CheckFaceParameter(rsIndex,faceIndex);
		recordCount=this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.nbRecords;
		vertexA=this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex[0];
		vertexB=this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex[1];
		vertexC=this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex[2];
	}

	float rsurf_data::ComputeFaceArea(const std::size_t& rsIndex,const std::size_t& faceIndex)
	{
		CheckFaceParameter(rsIndex,faceIndex);
		Intb* vertices(this->localData->tabRs[rsIndex].dataFaces[faceIndex].dataFace.sommetsIndex);
		return GetAireTriangle(this->localData->tabNodes[vertices[0]].node,this->localData->tabNodes[vertices[1]].node,this->localData->tabNodes[vertices[2]].node);
	}
	float rsurf_data::GetFaceSumEnergy(const std::size_t& rsIndex,const std::size_t& faceIndex)
	{
		CheckFaceParameter(rsIndex,faceIndex);
		double sumEnergy(0.);
		const t_ExchangeData_Face& curFace(this->localData->tabRs[rsIndex].dataFaces[faceIndex]);
		for(int recordIndex=0;recordIndex<curFace.dataFace.nbRecords;recordIndex++)
			sumEnergy+=curFace.tabTimeStep[recordIndex].energy;
		return sumEnergy;
	}
	void rsurf_data::GetFaceEnergy(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& recordIndex,std::size_t& idstep,float& energy) const
	{
		CheckRecordParameter(rsIndex,faceIndex,recordIndex);
		idstep=this->localData->tabRs[rsIndex].dataFaces[faceIndex].tabTimeStep[recordIndex].timeStep;
		energy=this->localData->tabRs[rsIndex].dataFaces[faceIndex].tabTimeStep[recordIndex].energy;
	}


	void rsurf_data::SetStruct(t_ExchangeData* extData)
	{
		delete this->localData;
		this->localData=extData;
	}

	t_ExchangeData* rsurf_data::GetStruct()
	{
		return this->localData;
	}

	rsurf_io::rsurf_io()
	{

	}

	bool rsurf_io::Load(const std::string& fileName,rsurf_data& data)
	{
		RSBIN readerBin;
		return readerBin.ImportBIN(fileName.c_str(),*data.GetStruct());
	}
	bool rsurf_io::Save(const std::string& fileName,rsurf_data& data)
	{
		RSBIN writerBin;
		return writerBin.ExportBIN(fileName.c_str(),*data.GetStruct());
	}

	/**
	 * Contient les données propre aux sommets des triangles
	 */
	struct t_tri_data
	{
		t_tri_data()
			:useInIsoLines(true)
		{

		}
		t_tri_data(const vec3& A,const vec3& B,const vec3& C,const float& Alvl,const float& Blvl,const float& Clvl)
		{
			useInIsoLines=true;
			vertices[0]=A;
			vertices[1]=B;
			vertices[2]=C;
			vertices_values[0]=Alvl;
			vertices_values[1]=Blvl;
			vertices_values[2]=Clvl;
		}
		vec3 vertices[3];
		float vertices_values[3];
		bool useInIsoLines;
	};

	struct t_iso_contouring_data
	{
		t_iso_contouring_data() :maxValue(0),minValue(0) {}
		float maxValue;
		float minValue;
		std::list<t_tri_data> tri_grid;
	};
	typedef std::list<t_tri_data>::iterator tri_grid_iterator;
	rsurf_iso_contouring::~rsurf_iso_contouring()
	{
		delete iso_contouring_data;
	}
	rsurf_iso_contouring::rsurf_iso_contouring(const rsurf_data& data,const std::size_t& rsIndex)
		:iso_contouring_data(new t_iso_contouring_data())
	{
		//Initialisation
		std::size_t tsSize(0);
		std::size_t nodesSize(0);
		std::size_t nbtimestep(0);
		std::size_t nbfaces(0);
		std::size_t idstep(0);
		std::size_t vertices[3]={0,0,0};
		std::size_t records(0);
		float timestep(0.f);
		float energy(0.f);
		float totenergy(0.f);
		std::string rs_name;
		std::string recordType;
		int rs_xmlid(0);
		bool converttodb=false;
		data.GetRsInfo(rsIndex,nbfaces,rs_name,rs_xmlid);

		data.GetFileInfos(tsSize,nodesSize,nbtimestep,timestep,recordType);
		if(recordType=="SPL_STANDART" || recordType=="SPL_GAIN")
			converttodb=true;
		//Passage de valeur de surface vers des valeurs de sommets
		std::vector<float> vertices_lvl(nodesSize,0.f);
		std::vector<unsigned int> vertices_countlinkedfaces(nodesSize,0);
		std::vector<bool> face_useforcalculation(nbfaces,true);
		for(std::size_t idface=0;idface<nbfaces;idface++)
		{
			data.GetFaceInfo(rsIndex,idface,vertices[0],vertices[1],vertices[2],records);

			//Cumul de l'energie sur le temps
			totenergy=0;
			for(std::size_t idrecord=0;idrecord<records;idrecord++)
			{
				data.GetFaceEnergy(rsIndex,idface,idrecord,idstep,energy);
				totenergy+=energy;
			}
			if(totenergy==0 || totenergy==std::numeric_limits<float>::infinity() || -totenergy==std::numeric_limits<float>::infinity())
			{
				face_useforcalculation[idface]=false;
			}else{
				vertices_countlinkedfaces[vertices[0]]+=1;
				vertices_countlinkedfaces[vertices[1]]+=1;
				vertices_countlinkedfaces[vertices[2]]+=1;
				vertices_lvl[vertices[0]]+=totenergy;
				vertices_lvl[vertices[1]]+=totenergy;
				vertices_lvl[vertices[2]]+=totenergy;
			}
		}
		//Division des valeurs par leurs nombre de contributeurs et conversion en dB
		for(std::size_t idvert=0;idvert<nodesSize;idvert++)
		{
			if(vertices_countlinkedfaces[idvert]>0)
				vertices_lvl[idvert]/=vertices_countlinkedfaces[idvert];
			if(converttodb && vertices_lvl[idvert]>0)
				vertices_lvl[idvert]=10*log10(vertices_lvl[idvert]/pow(10.f,-12.f));
		}
		//Initialisation des variables de la classe
		iso_contouring_data->minValue=vertices_lvl[vertices[0]];
		iso_contouring_data->maxValue=iso_contouring_data->minValue;
		vec3 A,B,C;
		for(std::size_t idface=0;idface<nbfaces;idface++)
		{
			data.GetFaceInfo(rsIndex,idface,vertices[0],vertices[1],vertices[2],records);
			data.GetNodePositionValue(vertices[0],A.x,A.y,A.z);
			data.GetNodePositionValue(vertices[1],B.x,B.y,B.z);
			data.GetNodePositionValue(vertices[2],C.x,C.y,C.z);
			t_tri_data newtri(A,B,C,vertices_lvl[vertices[0]],vertices_lvl[vertices[1]],vertices_lvl[vertices[2]]);
			newtri.useInIsoLines=face_useforcalculation[idface];
			iso_contouring_data->tri_grid.push_back(newtri);
			FINDMINMAX_RSURF(vertices_lvl[vertices[0]],vertices_lvl[vertices[1]],vertices_lvl[vertices[2]],iso_contouring_data->minValue,iso_contouring_data->maxValue);
		}
	}
	void rsurf_iso_contouring::GetMinMax(float& min,float& max)
	{
		min=iso_contouring_data->minValue;
		max=iso_contouring_data->maxValue;
	}
	float GetLvl(const vec3& dot,const vec3& A,const vec3& B,const vec3& C,const float& Alvl,const float& Blvl,const float& Clvl)
	{

		return 0.;
	}
	void rsurf_iso_contouring::SmoothGrid(const int& smoothcount)
	{
		vec3 MAB,MBC,MCA;
		float MABv,MBCv,MCAv;
		for(int smoothcounter=0;smoothcounter<smoothcount;smoothcounter++)
		{

			std::size_t nbtri=this->iso_contouring_data->tri_grid.size();
			std::size_t tri_count=0;
			for(tri_grid_iterator itface=this->iso_contouring_data->tri_grid.begin();tri_count<nbtri;itface++)
			{

				//Pour chaque triangle on subdivise en 4 triangles
				//           /\ 0 (A)
				//          /  \
				//   MCA   /----\ MAB
				//        / \  / \
				//2 (C)  /___\/___\ 1 (B)
				//          MBC
				//Calcul des valeurs intermédiaires
				const vec3 A=(*itface).vertices[0];
				const vec3 B=(*itface).vertices[1];
				const vec3 C=(*itface).vertices[2];
				const float Av=(*itface).vertices_values[0];
				const float Bv=(*itface).vertices_values[1];
				const float Cv=(*itface).vertices_values[2];
				//this->iso_contouring_data->tri_grid.erase(itface)
				MAB=(A+B)/2.f;
				MBC=(B+C)/2.f;
				MCA=(C+A)/2.f;
				MABv=(Av+Bv)/2.f;
				MBCv=(Bv+Cv)/2.f;
				MCAv=(Cv+Av)/2.f;
				//MABv=GetLvl(MAB,A,B,C,Av,Bv,Cv);
				//MBCv=GetLvl(MBC,A,B,C,Av,Bv,Cv);
				//MCAv=GetLvl(MCA,A,B,C,Av,Bv,Cv);
				//Modification du triangle d'origine
				(*itface).vertices[1]=MAB;
				(*itface).vertices_values[1]=MABv;
				(*itface).vertices[2]=MCA;
				(*itface).vertices_values[2]=MCAv;
				//Ajout du triangle MAB B MBC
				iso_contouring_data->tri_grid.push_back(t_tri_data(MAB,B,MBC,MABv,Bv,MBCv));
				//Ajout du triangle MCA MBC C
				iso_contouring_data->tri_grid.push_back(t_tri_data(MCA,MBC,C,MCAv,MBCv,Cv));
				//Ajout du trangle MCA MAB MBC
				iso_contouring_data->tri_grid.push_back(t_tri_data(MCA,MAB,MBC,MCAv,MABv,MBCv));
				tri_count++;
			}
		}
	}
	#define segmentCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,Av,Bv,Apos,Bpos) if(IS_VAL_BETWEEN(isolvl,Av,Bv)) { segmentCoordinates[segmentCoordinatesIndex]=Apos+(((Bpos-Apos)*(isolvl-Av))/(Bv-Av)); segmentCoordinatesIndex++; }
	#define segmentOnVertsCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,Av,Apos) if(isolvl==Av) { segmentCoordinates[segmentCoordinatesIndex]=Apos; segmentCoordinatesIndex++; }
	#define segmentOnSideCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,Av,Bv,Apos,Bpos) if(isolvl==Av && isolvl==Bv) { segmentCoordinates[0]=Apos;segmentCoordinates[1]=Bpos;segmentCoordinatesIndex=2; }

	void rsurf_iso_contouring::GetIsoLevelCurves(const float& isolvl, std::list<t_curve>& curvlst)
	{

		for(tri_grid_iterator itface=this->iso_contouring_data->tri_grid.begin();itface!=this->iso_contouring_data->tri_grid.end();itface++)
		{
			// Si les valeurs des sommets ne sont pas toutes au dessus ni en dessous
			if((*itface).useInIsoLines && !IS_OUT_OF_ISO(isolvl,(*itface).vertices_values[0],(*itface).vertices_values[1],(*itface).vertices_values[2]))
			{
				vec3 segmentCoordinates[2];
				unsigned short segmentCoordinatesIndex=0;
				//Test iso entre sommets
				//Segment AB
				segmentCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[0],(*itface).vertices_values[1],(*itface).vertices[0],(*itface).vertices[1])
				//Segment BC
				segmentCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[1],(*itface).vertices_values[2],(*itface).vertices[1],(*itface).vertices[2])
				//Segment CA
				segmentCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[2],(*itface).vertices_values[0],(*itface).vertices[2],(*itface).vertices[0])

				//Test iso courbe sur segments
				if(segmentCoordinatesIndex==0)
				{
					//Segment AB
					segmentOnSideCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[0],(*itface).vertices_values[1],(*itface).vertices[0],(*itface).vertices[1])
					//Segment BC
					segmentOnSideCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[1],(*itface).vertices_values[2],(*itface).vertices[1],(*itface).vertices[2])
					//Segment CA
					segmentOnSideCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[2],(*itface).vertices_values[0],(*itface).vertices[2],(*itface).vertices[0])
				}
				//Test iso courbe sur sommets
				if(segmentCoordinatesIndex!=2)
				{
					//Sommet A
					segmentOnVertsCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[0],(*itface).vertices[0])
					//Sommet B
					segmentOnVertsCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[1],(*itface).vertices[1])
					//Sommet C
					segmentOnVertsCoordinatesInit(segmentCoordinates,segmentCoordinatesIndex,isolvl,(*itface).vertices_values[2],(*itface).vertices[2])
				}
				if(segmentCoordinatesIndex==2)
				{
					//On a un segment
					t_curve seg;
					seg.A[0]=segmentCoordinates[0].x;
					seg.A[1]=segmentCoordinates[0].y;
					seg.A[2]=segmentCoordinates[0].z;
					seg.B[0]=segmentCoordinates[1].x;
					seg.B[1]=segmentCoordinates[1].y;
					seg.B[2]=segmentCoordinates[1].z;
					curvlst.push_back(seg);
				}
			}
		}
	}
};
