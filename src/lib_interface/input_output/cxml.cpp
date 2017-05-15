#include "cxml.h"
#include "utf8.h"
#include <algorithm> //trie de vector
#include <iostream>

bool NodeSortPredicate(CXmlNode* lEl,CXmlNode* rEl)
{
  return *lEl<*rEl;
}

///////////////////////////////////////////////
// CXmlNodeProperty
//
///////////////////////////////////////////////

CXmlNodeProperty::CXmlNodeProperty()
{
}
CXmlNodeProperty::CXmlNodeProperty(CXmlNodeProperty* propSrc)
{
	propSrc->GetData(propData);
}
CXmlNodeProperty::~CXmlNodeProperty()
{

}
CXmlNodeProperty::CXmlNodeProperty(xString name,xString value)
{
	propData.propName=name;
	propData.propValue=value;
}


xString CXmlNodeProperty::GetName() const
{
	return propData.propName;
}

xString CXmlNodeProperty::GetValue() const
{
	return propData.propValue;
}
void CXmlNodeProperty::GetData(t_PropData& data)
{
	data=propData;
}
///////////////////////////////////////////////
// CXmlNode
//
///////////////////////////////////////////////

CXmlNode::CXmlNode(CXmlNode* _parent , const pugi::xml_node& rowData )
{
	this->nodeData.parent=NULL;
	if(rowData!=NULL)
	{
	  nodeData.nodeName=rowData.name();
      for (auto &&item : rowData.attributes()) {
        AddProperty(item.name(), item.value());
      }
      for (auto &&child : rowData.children()) {
        CXmlNode* newChild = new CXmlNode(this, child);
        this->nodeData.children.push_back(newChild);
      }
	}
	this->nodeData.parent=_parent;
}

CXmlNode::~CXmlNode()
{
	//Destruction des objets
	for(unsigned long i=0;i<nodeData.children.size();i++)
		delete nodeData.children[i];

}

void CXmlNode::AddChildren( CXmlNode* newChildren )
{
	this->nodeData.children.push_back(newChildren);
}

void CXmlNode::ReParentNode( CXmlNode* newParent )
{
}

const std::vector<CXmlNode*>::iterator CXmlNode::GetFirstChild()
{
	return this->nodeData.children.begin();
}


const std::vector<CXmlNode*>::iterator CXmlNode::GetLastChild()
{
	return this->nodeData.children.end();
}


bool CXmlNode::IsPropertyExist(xString name)
{
	int indice=this->GetIndiceProp(name);
	if(indice>=0)
	{
		return true;
	}else{
		return false;
	}
}
xString CXmlNode::GetProperty(xString name)  const
{
	int indice=this->GetIndiceProp(name);
	if(indice>=0)
	{
		return nodeData.nodeProperties[indice].GetValue();
	}else{
		std::cout<<"Xml Property "<<name<<" doesn't exist !"<<std::endl;
		return xString();
	}
}

void CXmlNode::AddProperty( xString name, xString value )
{
	nodeData.nodeProperties.push_back(CXmlNodeProperty(name,value));
}

bool CXmlNode::DeleteProperty( xString name )
{
	return true;
}

void CXmlNode::OrderChildsByProperty( xString nameProperty, bool ascOrder)
{
	this->propertyNameSorting=nameProperty;
	std::sort(this->nodeData.children.begin( ), this->nodeData.children.end( ), NodeSortPredicate );
}
uentier CXmlNode::CountChildrens()
{
	return this->nodeData.children.size();
}
bool CXmlNode::operator>(const CXmlNode &_rEl)
{

	if(this->nodeData.parent)
	{
		return this->GetProperty(this->nodeData.parent->propertyNameSorting).ToInt()>_rEl.GetProperty(this->nodeData.parent->propertyNameSorting).ToInt();
	}
	return false;
}

bool CXmlNode::operator<(const CXmlNode &_rEl)
{
	if(this->nodeData.parent)
	{
		return this->GetProperty(this->nodeData.parent->propertyNameSorting).ToInt()<_rEl.GetProperty(this->nodeData.parent->propertyNameSorting).ToInt();
	}
	return false;
}

int CXmlNode::GetIndiceProp( xString name ) const
{
	for(std::size_t indice=0;indice<nodeData.nodeProperties.size();indice++)
		if(nodeData.nodeProperties[indice].GetName()==name)
			return indice;
	return -1;
}

xString CXmlNode::GetName() const
{
	return nodeData.nodeName;
}

CXmlNode* CXmlNode::GetChild(xString name)
{
	for(std::size_t indice=0;indice<nodeData.children.size();indice++)
	{
		if(nodeData.children[indice]->GetName()==name)
			return nodeData.children[indice];
	}
	return NULL;
}
///////////////////////////////////////////////
// CXml
//
///////////////////////////////////////////////
CXml::CXml( xString path )
{
	rootNode = NULL;
	_ClearDocument();

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  if(result) {
    rootNode = new CXmlNode(NULL, doc.first_child());
  }
}

CXml::~CXml()
{
	_ClearDocument();
}
CXml::CXml( )
{
	rootNode = NULL;
	_ClearDocument();


}

CXmlNode* CXml::GetRoot()
{
	return rootNode;
}

void CXml::_ClearDocument()
{
	delete rootNode;
	rootNode=NULL;
}

