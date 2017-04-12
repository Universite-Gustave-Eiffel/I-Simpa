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
% *   sour   : Vecteur "mat_octave".
% *
% *   SOUS-PROGRAMMES :
% *
% *   LINE2D
% *
% c----$---1---------2---------3---------4---------5---------6---------7-c
 function [mat_100, mat_125, mat_160, mat_200, ...
   mat_250, mat_315, mat_400, mat_500,mat_630,...
   mat_800, mat_1k,mat_1250,mat_1600, ...
   mat_2k,mat_2500,mat_3150, mat_4k, mat_5k]= fluxFronTriang2d_oct(abs_prop,x,y,z,nd,kne,mat, face_mat)
% indice DE 4 à 20: 100Hz à 5kHz
 

%	' On integre w sur la surface du volume de controle 3D qui est une portion de triangle 2D 
% composï¿½e de deux sous triangles 'T1+T2'
%mat_400, mat_500,mat_630,mat_800, mat_1k,mat_1250=mat,mat_1600, mat_2k,mat_2500,mat_3150, mat_4k, mat_5k
%=======================
mat_100=mat; mat_125=mat; mat_160=mat; 
mat_200=mat; mat_250=mat; mat_315=mat; 
mat_400=mat; mat_500=mat; mat_630=mat; 
mat_800=mat; mat_1k=mat; mat_1250=mat;
mat_1600=mat; mat_2k=mat; mat_2500=mat;
mat_3150=mat; mat_4k=mat; mat_5k=mat;

el2=0;
c0=343;

for n=1:nd
              
              ks=kne(n,:);  XA=[x(ks(1)); y(ks(1)); z(ks(1))];
              XB=[x(ks(2)); y(ks(2)); z(ks(2))];     XC=[x(ks(3)); y(ks(3)); z(ks(3))];
                     el2=el2+1;
      % Calcul de l'aire du Triangle
            AireTr= 1/2*norm(cross((XB-XA), (XC-XA)));
      % Coefficients de contribution
    
     idm= find((abs_prop(:,1)-face_mat(n))==0);% indice matériau
      
                
               h=c0/4*abs_prop(idm,5);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_100]= matriceflux_oct(ai,ajk, ks,mat_100);
              
                     h=c0/4*abs_prop(idm,6);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_125]= matriceflux_oct(ai,ajk, ks,mat_125);

             h=c0/4*abs_prop(idm,7);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_160]= matriceflux_oct(ai,ajk, ks,mat_160);


              h=c0/4*abs_prop(idm,8);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_200]= matriceflux_oct(ai,ajk, ks,mat_200);
              
             h=c0/4*abs_prop(idm,9);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_250]= matriceflux_oct(ai,ajk, ks,mat_250);
              
              
              h=c0/4*abs_prop(idm,10);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_315]= matriceflux_oct(ai,ajk, ks,mat_315);

               h=c0/4*abs_prop(idm,11);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_400]= matriceflux_oct(ai,ajk, ks,mat_400);
              
             h=c0/4*abs_prop(idm,12);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_500]= matriceflux_oct(ai,ajk, ks,mat_500);
              
              
              h=c0/4*abs_prop(idm,13);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_630]= matriceflux_oct(ai,ajk, ks,mat_630);

              h=c0/4*abs_prop(idm,14);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_800]= matriceflux_oct(ai,ajk, ks,mat_800);              
              
             h=c0/4*abs_prop(idm,15);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_1k]= matriceflux_oct(ai,ajk, ks,mat_1k);       
              
             h=c0/4*abs_prop(idm,16);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_1250]= matriceflux_oct(ai,ajk, ks,mat_1250);              
               
             h=c0/4*abs_prop(idm,17);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_1600]= matriceflux_oct(ai,ajk, ks,mat_1600);              

              h=c0/4*abs_prop(idm,18);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_2k]= matriceflux_oct(ai,ajk, ks,mat_2k);      
      
             h=c0/4*abs_prop(idm,19);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_2500]= matriceflux_oct(ai,ajk, ks,mat_2500);              
      
               h=c0/4*abs_prop(idm,20);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_3150]= matriceflux_oct(ai,ajk, ks,mat_3150);              

    
             h=c0/4*abs_prop(idm,21);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_4k]= matriceflux_oct(ai,ajk, ks,mat_4k);              
    
               h=c0/4*abs_prop(idm,22);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_5k]= matriceflux_oct(ai,ajk, ks,mat_5k);        
              
              
%                   mat_octave(ks(1),ks(2))=ajk+mat_octave(ks(1),ks(2));
%                   mat_octave(ks(1),ks(3))=ajk+mat_octave(ks(1),ks(3));
%                   mat_octave(ks(1),ks(1))=ai+mat_octave(ks(1),ks(1));
%
%                   mat_octave(ks(2),ks(1))=ajk+mat_octave(ks(2),ks(1));
%                   mat_octave(ks(2),ks(3))=ajk+mat_octave(ks(2),ks(3));
%                   mat_octave(ks(2),ks(2))=ai+mat_octave(ks(2),ks(2));
%              
%                   mat_octave(ks(3),ks(1))=ajk+mat_octave(ks(3),ks(1));
%                   mat_octave(ks(3),ks(2))=ajk+mat_octave(ks(3),ks(2));
%                   mat_octave(ks(3),ks(3))=ai+mat_octave(ks(3),ks(3));
              end
            

end
%======================

