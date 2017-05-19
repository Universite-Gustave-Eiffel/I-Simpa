% *   Calcul des coefficients de la matrice 'SOUR'.
% pour le flux au travers des parois du local 
% sur les elements surfaciques (triangles )
% *   On integre sur les deux sous-triangle S1=S/4 et S2=S/12
% *   ENTREE :
% *
% *   x,y,z    : Tableaux des coordonnees.
% *   kne    : Tableau des noeuds associes a chaque element.
% *   nd     : Nombre maximum d'elements associes a un type.
% *   ne1    : Nombre maximum de noeuds.
% *   ne2    : Dimension de 'kne'.
% *
% *   SORTIE :
% *
% *   sour   : Vecteur "source".
% *
% *   SOUS-PROGRAMMES :
% *
% *   LINE2D
% *
% c----$---1---------2---------3---------4---------5---------6---------7-c
 function [source]= fluxFronTriang2d(h,x,y,z,nd,kne,source)

 %source=zeros(ne1,ne1);
 %xn=zeros(3,1);yn=xn; zn=xn;% ks=xn;
%a=zeros(4,1);b=a;c=a;

%	' On integre w sur la surface du volume de controle 3D qui est une portion de triangle 2D 
% compos�e de deux sous triangles 'T1+T2'

%=======================
el2=0;
for n=1:nd
              
              ks=kne(n,:);
              XA=[x(ks(1)); y(ks(1)); z(ks(1))];
              XB=[x(ks(2)); y(ks(2)); z(ks(2))];
              XC=[x(ks(3)); y(ks(3)); z(ks(3))];
                     el2=el2+1;
      % Calcul de l'aire du Triangle
            AireTr= 1/2*norm(cross((XB-XA), (XC-XA)));
      % Coefficients de contribution
      
              ai=22./108*h*AireTr;
              ajk=7/108*h*AireTr; 
              
                   source(ks(1),ks(2))=ajk+source(ks(1),ks(2));
                   source(ks(1),ks(3))=ajk+source(ks(1),ks(3));
                   source(ks(1),ks(1))=ai+source(ks(1),ks(1));

                   source(ks(2),ks(1))=ajk+source(ks(2),ks(1));
                   source(ks(2),ks(3))=ajk+source(ks(2),ks(3));
                   source(ks(2),ks(2))=ai+source(ks(2),ks(2));
              
                   source(ks(3),ks(1))=ajk+source(ks(3),ks(1));
                   source(ks(3),ks(2))=ajk+source(ks(3),ks(2));
                   source(ks(3),ks(3))=ai+source(ks(3),ks(3));
              end
            

end
%======================

