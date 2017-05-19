%   Construction de la matrice M_int_Lin.
%   On integre sur le sous-Tetrahedre de volume v/4 
%   ENTREE :
%   x,y,z   : Tableaux des coordonnees.
%   kne    : Tableau des noeuds associes a chaque element.
%   nbel     : Nombre d'elements Tetra.
%   nn    : Nombre de noeuds.


% *
% *   SORTIE :
% *
% *   Matrice des coefficient de l'integration par interpolation lineaire
% * sur des Tetrahedres.
% *
% *   SOUS-PROGRAMMES :
% *
% *   volumeTetra
% *
% c----$---1---------2---------3---------4---------5---------6---------7-c

function [M_int_Lin]= integVC_lin(x,y,z,nn,nbel,kne)

         ia = [];        ja = [];        s = [];
         M_int_Lin = sparse ( ia, ja, s, nn, nn,15 );

%xn=zeros(4,1);yn=xn; zn=xn; %ks=xn;
  

for n=1:nbel
     ks=kne(n,:);
     xn=x(ks);     yn =y(ks);   zn =z(ks);
    [v]=volumeTetra(xn,yn,zn);%Portion du VC dans le Tetra;
    Cvois=23.*v/576.;%Coeff des voisins;
    Cpt=75.*v/576.;%Coeff pt;
  
    M_int_Lin(ks(1),ks(1))=Cpt+M_int_Lin(ks(1),ks(1));
    M_int_Lin(ks(1),ks(2))=Cvois+M_int_Lin(ks(1),ks(2));
    M_int_Lin(ks(1),ks(3))=Cvois+M_int_Lin(ks(1),ks(3));
    M_int_Lin(ks(1),ks(4))=Cvois+M_int_Lin(ks(1),ks(4));
                
                
    M_int_Lin(ks(2),ks(1))=Cvois+M_int_Lin(ks(2),ks(1));
    M_int_Lin(ks(2),ks(2))=Cpt+M_int_Lin(ks(2),ks(2));
    M_int_Lin(ks(2),ks(3))=Cvois+M_int_Lin(ks(2),ks(3));
    M_int_Lin(ks(2),ks(4))=Cvois+M_int_Lin(ks(2),ks(4));
                          
    M_int_Lin(ks(3),ks(1))=Cvois+M_int_Lin(ks(3),ks(1));
    M_int_Lin(ks(3),ks(2))=Cvois+M_int_Lin(ks(3),ks(2));
    M_int_Lin(ks(3),ks(3))=Cpt+M_int_Lin(ks(3),ks(3));
    M_int_Lin(ks(3),ks(4))=Cvois+M_int_Lin(ks(3),ks(4));
                          
    M_int_Lin(ks(4),ks(1))=Cvois+M_int_Lin(ks(4),ks(1));
    M_int_Lin(ks(4),ks(2))=Cvois+M_int_Lin(ks(4),ks(2));
    M_int_Lin(ks(4),ks(3))=Cvois+M_int_Lin(ks(4),ks(3));
    M_int_Lin(ks(4),ks(4))=Cpt+M_int_Lin(ks(4),ks(4));

end

  end
  

                
%===========================================================

