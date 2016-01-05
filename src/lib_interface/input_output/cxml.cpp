#include "cxml.h"
#include <stdio.h>
#include "utf8.h"
#include <algorithm> //trie de vector
#include <iostream>

namespace BasicXmlProcess
{
/* Licence des méthodes dans l'espace de nom BasicXmlProcess*/
/*
Copyright (c) 2003 Daniel W. Howard

Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the
Software without restriction, including without
limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions of
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdio.h>
#include <string.h>

/* deletebasicxmlnode: frees all memory for xml tree */
void deletebasicxmlnode( struct basicxmlnode * node )
{
  int ii; if (!node) return;
  for (ii=0; node->children[ii]; ++ii)
    deletebasicxmlnode(node->children[ii]);
  delete node->tag; /*free(node->tag);*/
  delete node->text; /*free(node->text);*/
  for (ii=0; node->attrs[ii]; ++ii) {
    delete [] node->attrs[ii]; /*free(node->attrs[ii]);*/
    delete [] node->values[ii]; /*free(node->values[ii]);*/
  }
  delete [] node->attrs; /*free(node->attrs);*/
  delete [] node->values; /*free(node->values);*/
  delete [] node->children; /*free(node->children);*/
  delete [] node->childreni; /*free(node->childreni);*/
  delete node; /*free(node);*/
}

/* readbasicxmlnode: reads simple XML file */
struct basicxmlnode * readbasicxmlnode( FILE * fpi )
{
  int bufinc=500, textinc=50, childinc=50; /* tuning */
  char * buf=0; int nbuf=0;
  char * ibuf=0, * buftmp, * wspos, * ps, * pe, * pe2;
  int phase=0, ch=0, err=0, ii=0, ia, natts, * nodeitmp;
  int itex=0, ntex=textinc, ichi=0, nchi=childinc, square=0;
  typedef /*struct*/ basicxmlnode nodetype;
  nodetype * node, * child, * * nodetmp;
  size_t so_c = sizeof(char), so_cp = sizeof(char *);
  size_t so_i = sizeof(int);
  size_t so_n = sizeof(struct basicxmlnode);
  size_t so_np = sizeof(struct basicxmlnode *);
  if (!err && !fpi) err=1; /* bad arg */
  /* get chars between < and >; strip most whitespace */
  while (1) {
    int isws=0, issl=0, islt=0, isgt=0, iseq=0, isqu=0;
    int isqs=0, isex=0, isda=0, issi=0, isun=0, ispe=0;
    int islb=0, isrb=0, isn1=0, isn2=0;
    if ((ibuf-buf+5) > nbuf) { /* realloc buf */
      nbuf += bufinc;
      buftmp = new char[nbuf]; /*(char*)malloc(so_c*nbuf);*/
      if (!buftmp) {
         err=2; break; /* out of memory */
      }
      if (buf) {
        ii = ibuf-buf;
        strncpy(buftmp, buf, ii);
        delete [] buf; /*free(buf);*/
      }
      buf = buftmp;
      ibuf = buf + ii;
    }
    if (phase != 100) ch = fgetc(fpi);
    if (err || feof(fpi) || (phase == 100)) break;
    /* analyze char */ /* /<>="?!-'12*/
    if ((ch == ' ') || ((ch > 8) && (ch < 14))) isws = 1;
    if (ch == '/') issl = 1; if (ch == '<') islt = 1;
    if (ch == '>') isgt = 1; if (ch == '=') iseq = 1;
    if (ch == '"') isqu = 1; if (ch == '?') isqs = 1;
    if (ch == '!') isex = 1; if (ch == '-') isda = 1;
    if (ch == '\'') issi = 1; if (ch == '_') isun = 1;
    if (ch == '[') islb = 1; if (ch == ']') isrb = 1;
    if (ch == '.') ispe = 1;
    if ((ch >= 'a') && (ch <= 'z')) isn1 = 1;
    if ((ch >= 'A') && (ch <= 'Z')) isn1 = 1;
    if (isun) isn1 = 1;
    if (isn1 || isda || ispe) isn2 = 1;
    if ((ch >= '0') && (ch <= '9')) isn2 = 1;
    /* err=3, bad xml file (parsing error) */
    switch (phase) {
      case 0: /* eat whitespace before < */
        if (isws) { } /* */
        else if (islt) { ++phase; *ibuf=ch; ++ibuf; } /*<*/
        else { err=3; } /**/ /*/>="?!-'t1*/
        break;
      case 1: /* handle the first char after < */
        if (issl) { phase=10; *ibuf=ch; ++ibuf; } /*</*/
        else if (isqs) { phase=14; --ibuf; } /*<?*/
        else if (isex) { phase=15; --ibuf; } /*<!*/
        else if (isn1) { ++phase; *ibuf=ch; ++ibuf; } /*<t*/
        else { err=3; } /*<*/ /* <>="-'1*/
        break;
      case 2: /* store tag name and a single space after it */
        if (isws) { ++phase; *ibuf = ' '; ++ibuf; } /*<t */
        else if (issl) { phase=13; strncpy(ibuf," /",2); ibuf+=2; } /*<t/*/
        else if (isgt) { phase=100; strncpy(ibuf," >",2); ibuf+=2; } /*<t>*/
        else if (isn2) { *ibuf=ch; ++ibuf; } /*<t*/ /*-1*/
        else { err=3; } /*<t*/ /*<="?!'*/
        break;
      case 3: /* eat whitespace after tag name */
        if (isws) { } /*<tag  */
        else if (issl) { phase=13; *ibuf=ch; ++ibuf; } /*<tag /*/
        else if (isgt) { phase=100; *ibuf=ch; ++ibuf; } /*<tag >*/
        else if (isn1) { ++phase; *ibuf=ch; ++ibuf; } /*<tag t*/
        else { err=3; } /*<tag */ /*<="?!-'1*/
        break;
      case 4: /* store attribute name */
        if (isws) { ++phase; } /*<tag a */
        else if (iseq) { phase=6; *ibuf=ch; ++ibuf; } /*<tag a=*/
        else if (isn2) { *ibuf=ch; ++ibuf; } /*<tag a*/ /*-t1*/
        else { err=3; } /*<tag a*/ /*/<>"?!'t*/
        break;
      case 5: /* eat whitespace after attribute name */
        if (isws) { } /*<tag attr */
        else if (iseq) { ++phase; *ibuf=ch; ++ibuf; } /*<tag attr =*/
        else { err=3; } /*<tag attr */ /*/<>"?!-'t1*/
        break;
      case 6: /* eat whitespace after = */
        if (isws) { } /*<tag attr= */
        else if (isqu) { ++phase; *ibuf=ch; ++ibuf; } /*<tag attr="*/
        else if (issi) { phase=8; *ibuf=ch; ++ibuf; } /*<tag attr='*/
        else { err=3; } /*<tag attr=*/ /*/<>=?!-t1*/
        break;
      case 7: /* store the attribute value ("value") */
        if (isws) { *ibuf=' '; ++ibuf; } /*tag attr=" */
        else if (islt) { err=3; } /*<tag attr="<*/
        else if (isqu) { phase=9; *ibuf=ch; ++ibuf; } /*<tag attr=""*/
        else { *ibuf=ch; ++ibuf; } /*tag attr="*/ /*/>=?!-'t1*/
        break;
      case 8: /* store the attribute value ('value') */
        if (isws) { *ibuf=' '; ++ibuf; } /*<tag attr=' */
        else if (islt) { err=3; } /*<tag attr='<*/
        else if (issi) { ++phase; *ibuf=ch; ++ibuf; } /*<tag attr=''*/
        else { *ibuf=ch; ++ibuf; } /*tag attr='*/ /*/>="?!-t1*/
        break;
      case 9: /* eat whitespace after attribute value */
        if (isws) { phase=3; *ibuf=' '; ++ibuf; } /*<tag attr="value" */
        else if (issl) { phase=13; strncpy(ibuf," /",2); ibuf+=2; } /*..."/*/
        else if (isgt) { phase=100; strncpy(ibuf," >",2); ibuf+=2; } /*...">*/
        else { err=3; } /*tag attr="value"*/ /*<="?!-'t1*/
        break;
      case 10: /* handle the first char after </ */
        if (isn1) { ++phase; *ibuf=ch; ++ibuf; } /*</t*/
        else { err=3; } /*</*/ /* /<>="?!-'1*/
        break;
      case 11: /* store tag name and a single space after it */
        if (isws) { ++phase; *ibuf=' '; ++ibuf; } /*</t */
        else if (isgt) { phase=100; strncpy(ibuf," >",2); ibuf+=2; } /*</t>*/
        else if (isn2) { *ibuf=ch; ++ibuf; } /*</t*/ /*-t*/
        else { err=3; } /*</t*/ /*/<="?!'*/
        break;
      case 12: /* eat whitespace before > */
        if (isws) { } /*</tag */
        else if (isgt) { phase=100; *ibuf=ch; ++ibuf; } /*</tag >*/
        else { err=3; } /*</tag */ /*/<="?!-'t1*/
        break;
      case 13: /* ensure final > */
        if (isgt) { phase=100; *ibuf=ch; ++ibuf; } /*<tag />*/
        else { err=3; } /*<tag /*/ /* /<="?!-'t1*/
        break;
      case 14: /* skip tag (<? tag) */
        if (isgt) { phase=0; } /*<?>*/
        else { } /*<?*/ /* /<="?!-'t1*/
        break;
      case 15: /* find first - (<! tag) */
        if (isgt) { phase=0; } /*<!>*/
        else if (isda) { ++phase; } /*<!-*/
        else if (islb) { ++square; phase=20; } /*<![*/
        else { phase=14; } /*<!*/ /* /<="?!'t1*/
        break;
      case 16: /* find second - (<!- tag) */
        if (isgt) { phase=0; } /*<!->*/
        else if (isda) { ++phase; } /*<!--*/
        else { phase=14; } /*<!-*/ /* /<="?!'t1*/
        break;
      case 17: /* skip comment */
        if (isda) { ++phase; } /*-*/
        else { } /**/ /* /<>="?!'t1*/
        break;
      case 18: /* find second - in --> */
        if (isda) { ++phase; } /*--*/
        else { phase=17; } /*-*/ /* /<>="?!'t1*/
        break;
      case 19: /* find > in --> */
        if (isgt) { phase=0; } /*-->*/
        else { err=3; } /*--*/ /* /<="?!-'t1*/
        break;
      case 20: /* find ] in <![ */
        if (islb) { ++square; }
        if (isrb) { if (--square==0) ++phase; }
        else { } /**/ /* /<>="?!'t1*/
        break;
      case 21: /* find > in (<![CDATA[]] tag) */
        if (isgt) { phase=0; } /*<?>*/
        else { } /*<?*/ /* /<="?!-'t1*/
        break;
    };
    if (err) break;
  }
  if (!err && (phase != 100)) {
    err=4; /* incomplete xml file */
  }
  if (!err) *ibuf = 0;
  /* buf is now in one of these formats: */
  /* <tag > or </tag > or <tag /> or */
  /* <tag attr="value" ... attr="value" > or */
  /* <tag attr="value" ... attr="value" /> */
  /* allocate memory for a node */
  node = new nodetype; /*(nodetype*)malloc(so_n);*/
  if (node) {
    node->tag = node->text = 0;
    node->attrs = node->values = 0;
    node->children = 0;
    node->childreni = 0;
  }
  if (!err) {
    if (!node) {
      err=5; /* out of memory */
    } else {
      memset(node, 0, sizeof(node));
  } }
  /* put tag name into node */
  if (!err) {
    wspos = strchr(buf, ' ');
    node->tag = new char[wspos-buf]; /*(char*)malloc(so_c*(wspos-buf));*/
    if (!node->tag) err=6; /* out of memory */
  }
  if (!err) {
    strncpy(node->tag, buf+1, wspos-buf-1);
    node->tag[wspos-buf-1] = 0;
    if (buf[1] == '/') { /* end tag (not err) */
      delete [] buf; /*free(buf);*/
      return node;
    }
    /* estimate # of attrs and values (maybe too much) */
    pe = strstr(wspos, "\" ");
    pe2 = strstr(wspos, "' ");
    for (natts=0; pe || pe2; ++natts) {
      if (!pe || (pe2 && (pe2 < pe))) pe = pe2;
      pe2 = strstr(pe+1, "' "); pe = strstr(pe+1, "\" ");
    }
    node->attrs = new char *[natts+1]; /*(char**)malloc(so_cp*(natts+1));*/
    node->values = new char *[natts+1]; /*(char**)malloc(so_cp*(natts+1));*/
    if (!node->attrs||!node->values) {
      if (node->attrs) delete [] node->attrs; /*free(node->attrs);*/
      if (node->values) delete [] node->values; /*free(node->values);*/
      node->attrs=node->values=0; err=7; /* out of memory */
  } }
  /* put attrs and values into node (and recount #) */
  if (!err) { ps = wspos+1;
    pe = strstr(wspos, "=\""); pe2 = strstr(wspos, "='");
  }
  for (natts=0, ia=0; !err && (pe||pe2); ++ia, ++natts) {
    if (!pe || (pe2 && (pe2 < pe))) pe = pe2;
    node->attrs[ia] = new char[pe-ps+1]; /*(char*)malloc(so_c*(pe-ps+1));*/
    if (!node->attrs[ia]) {
      node->values[ia]=0; err=8; break; /* out of memory */
    }
    strncpy(node->attrs[ia], ps, pe-ps);
    node->attrs[ia][pe-ps] = 0;
    node->attrs[ia+1] = 0;
    ps = pe + 2;
    if (pe == pe2) pe = strstr(ps, "' ");
    else pe = strstr(ps, "\" ");
    node->values[ia] = new char[pe-ps+1]; /*(char*)malloc(so_c*(pe-ps+1));*/
    if (!node->values[ia]) { err=9; break; } /* out of memory */
    strncpy(node->values[ia], ps, pe-ps);
    node->values[ia][pe-ps] = 0;
    node->values[ia+1] = 0;
    ps = pe + 2;
    pe2 = strstr(ps, "='"); pe = strstr(ps, "=\"");
  }
  if (!err) {
    node->attrs[natts] = node->values[natts] = 0;
    /* no children for self-contained tag, save memory */
    if (buf[strlen(buf)-2] == '/') {
      ntex = 1; nchi = 1;
    }
    /* no body or children yet */
    node->text = new char[ntex]; /*(char*)malloc(so_c*ntex);*/
    node->children = new nodetype *[nchi]; /*(nodetype**)malloc(so_np*nchi);*/
    node->childreni = new int [nchi]; /*(int*)malloc(so_i*nchi);*/
    if (!node->text || !node->children || !node->childreni) {
      err=10; /* out of memory */
    } else {
      node->text[0] = 0; node->children[0] = 0; node->childreni[0] = 0;
  } }
  if (!err) {
    if (ntex == 1) { /* end tag, no children (not err) */
      delete [] buf; /*free(buf);*/
      return node;
  } }
  /* read child tags */
  for (ichi=0; !err; ++ichi) {
    /* add more text to node (between child tags) */
    for (ch=fgetc(fpi); ch != '<'; ch=fgetc(fpi)) {
      if (feof(fpi)) {
        err=11; /* incomplete xml file */
        break;
      }
      if ((itex+2) == ntex) { /* realloc text */
        ntex += textinc;
        buftmp = new char[ntex]; /*(char*)malloc(so_c*ntex);*/
        if (!buftmp) { err=12; break; } /* out of memory */
        strncpy(buftmp, node->text, itex);
        delete [] node->text; /*free(node->text);*/
        node->text = buftmp;
      }
      node->text[itex++] = ch;
    }
    if (err) continue;
    node->text[itex] = 0;
    ungetc(ch, fpi); /* oops, put back next tag's "<" */
    child = readbasicxmlnode(fpi);
    if (child) {
      if (child->tag[0] == '/') {
        if (strcmp(child->tag+1, node->tag)) {
          err=13; /* end tag mismatch */
        }
        delete [] child->tag; /*free(child->tag);*/
        delete child; /*free(child);*/
        child=0;
    } } else if (!err) {
      err=14; /* bad child */
    }
    if (err || !child) break;
    node->children[ichi] = child;
    node->childreni[ichi] = itex;
    if ((ichi+2) == nchi) { /* realloc child tags array */
      nchi += childinc;
      nodetmp = new nodetype *[nchi]; /*(nodetype**)malloc(so_np*nchi);*/
      if (!nodetmp) { err=15; continue; } /* out of memory */
      nodeitmp = new int [nchi]; /*(int*)malloc(so_i*nchi);*/
      if (!nodeitmp) {
        delete [] nodetmp; /*free(nodetmp);*/
        nodetmp=0; err=16; continue; /* out of memory */
      }
      for (ii=0; ii <= ichi; ++ii) { /* copy */
        nodetmp[ii] = node->children[ii];
        nodeitmp[ii] = node->childreni[ii];
      }
      delete [] node->children; /*free(node->children);*/
      delete [] node->childreni; /*free(node->childreni);*/
      node->children = nodetmp;
      node->childreni = nodeitmp;
  } }
  if (node) {
    if (node->children) {
      node->children[ichi] = 0;
      node->childreni[ichi] = 0;
      if (err) { /* delete complete node */
        deletebasicxmlnode(node); node = 0;
    } } else { /* delete partial node */
      if (node->tag) delete [] node->tag; /*free(node->tag);*/
      if (node->text) delete [] node->text; /*free(node->text);*/
      if (node->attrs) {
        for (ii=0; node->attrs[ii]; ++ii) {
          delete [] node->attrs[ii]; /*free(node->attrs[ii]);*/
      } }
      if (node->values) {
        for (ii=0; node->values[ii]; ++ii) {
          delete [] node->values[ii]; /*free(node->values[ii]);*/
      } }
      if (node->attrs) delete node->attrs; /*free(node->attrs);*/
      if (node->values) delete node->values; /*free(node->values);*/
      delete node; /*free(node);*/
      node = 0;
  } }
  if (buf) delete [] buf; /*free(buf);*/
  return node; /* whew! return complete node */
}

/* printbasicxmlnode: prints to console */
void printbasicxmlnode( struct basicxmlnode * node )
{
  int ii, ch, it=0, haschildren=0; if (!node) return;
  if (node->children[0]) haschildren = 1;
  printf("<%s", node->tag); /* print tag */
  for (ii=0; node->attrs[ii]; ++ii) { /* print attrs */
    printf(" %s=", node->attrs[ii]);
    ch = strstr(node->values[ii], "\"")? '\'': '"';
    printf("%c%s%c", ch, node->values[ii], ch);
  }
  if (!haschildren && !node->text[0]) printf(" /");
  printf(">");
  for (ii=0; node->children[ii]; ++ii) { /* print children */
    while (it < node->childreni[ii]) /* print text */
      printf("%c", node->text[it++]);
    printbasicxmlnode(node->children[ii]);
  }
  printf("%s", node->text + it); /* print remaining text */
  if (haschildren || node->text[0]) {
    printf("</%s>", node->tag); /* print end tag */
  }
}

/* printbasicxmlnodetagnames: visits each node once */
void printbasicxmlnodetagnames( struct basicxmlnode * node )
{
  int sp=0, n;
  int stack[100];
  struct basicxmlnode * root = node;

  while (1) {
    printf("%s\n", node->tag);
    /* find the next node */
    stack[sp++] = 0;
    while (1) {
      node = root; /* find the parent */
      for (n=0; n < (sp-1); ++n) {
        node = node->children[stack[n]];
      }
      for (n=0; node->children[n]; ++n) ;
      if (stack[sp-1] != n) break;
      if (--sp == 0) break;
      ++stack[sp-1];
    }
    if (sp == 0) break;
    node = node->children[stack[sp-1]];
  }
}
}


using namespace BasicXmlProcess;

bool NodeSortPredicate(CXmlNode* lEl,CXmlNode* rEl)
{
  return *lEl<*rEl;
}
/*
int main()
{
  struct basicxmlnode * root = NULL;
  FILE * fp = fopen("file.xml", "rt");
  if (!fp) { printf("open failed\n"); return 1; }
  root = readbasicxmlnode(fp);
  fclose(fp);
  if (!root) { printf("read failed\n"); return 2; }
  printbasicxmlnode(root);
  deletebasicxmlnode(root);
  return 0;
}
*/

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

CXmlNode::CXmlNode(CXmlNode* _parent , BasicXmlProcess::basicxmlnode* rowData )
{
	this->nodeData.parent=NULL;
	if(rowData!=NULL)
	{
	  int ii, ch, it=0, haschildren=0;
	  if (rowData->children[0]) haschildren = 1;
	  nodeData.nodeName=rowData->tag;
	  for (ii=0; rowData->attrs[ii]; ++ii) { /* print attrs */
		  AddProperty(rowData->attrs[ii],rowData->values[ii]);
	  }
	  for (ii=0; rowData->children[ii]; ++ii)
	  {
		CXmlNode* nouvFils = new CXmlNode(this,rowData->children[ii]);
		this->nodeData.childrens.push_back(nouvFils);
	  }
	}
	this->nodeData.parent=_parent;
}

CXmlNode::~CXmlNode()
{
	//Destruction des objets
	for(unsigned long i=0;i<nodeData.childrens.size();i++)
		delete nodeData.childrens[i];

}

void CXmlNode::AddChildren( CXmlNode* newChildren )
{
	this->nodeData.childrens.push_back(newChildren);
}

void CXmlNode::ReParentNode( CXmlNode* newParent )
{
}

const std::vector<CXmlNode*>::iterator CXmlNode::GetFirstChild()
{
	return this->nodeData.childrens.begin();
}


const std::vector<CXmlNode*>::iterator CXmlNode::GetLastChild()
{
	return this->nodeData.childrens.end();
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
	std::sort(this->nodeData.childrens.begin( ), this->nodeData.childrens.end( ), NodeSortPredicate );
}
uentier CXmlNode::CountChildrens()
{
	return this->nodeData.childrens.size();
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
	for(std::size_t indice=0;indice<nodeData.childrens.size();indice++)
	{
		if(nodeData.childrens[indice]->GetName()==name)
			return nodeData.childrens[indice];
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

  struct basicxmlnode * root = NULL;
  FILE * fp = fopen(path.c_str(), "rt");
  if (!fp) { printf("open failed\n"); return; }
  root = readbasicxmlnode(fp);
  fclose(fp);
  if (!root) { printf("read failed\n"); return; }
  rootNode = new CXmlNode(NULL,root);
  deletebasicxmlnode(root);
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

