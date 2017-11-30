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
% function [mat_100, mat_125, mat_160, mat_200, ...
%   mat_250, mat_315, mat_400, mat_500,mat_630,...
%   mat_800, mat_1k,mat_1250,mat_1600, ...
%   mat_2k,mat_2500,mat_3150, mat_4k, mat_5k]= ...
    function [mat_Toct100Hz_5k]=...
   fluxFronT_oct(mat_Toct100Hz_5k,abs_prop,x,y,z,nd,NBLOCKS,eld2i..
   face_mat,nel2dT,el2dT,i_pts_Trsm, Transm)
% indice DE 4 à 20: 100Hz à 5kHz
 %fluxFronTriang2d_oct(abs_prop,x,y,z,nel2D,el2D,mat,face_mat)

%	' On integre w sur la surface du volume de controle 3D qui est une portion de triangle 2D 
% composï¿½e de deux sous triangles 'T1+T2'
%mat_400, mat_500,mat_630,mat_800, mat_1k,mat_1250=mat,mat_1600, mat_2k,mat_2500,mat_3150, mat_4k, mat_5k
c0=343;


el2=0;
%=======================+++++++++++++++++++++++++++++
%                                                     ABSORPTION
%=======================+++++++++++++++++++++++++++++
% Boucle sur les éléments 2D 
for ib=1:NBLOCKS
for n=1:size(el2di{ib},1)
              
              ks=el2di{ib}(n,:);  
              face_mat=ks(4);
              XA=[x(ks(1)); y(ks(1)); z(ks(1))];
              XB=[x(ks(2)); y(ks(2)); z(ks(2))];
              XC=[x(ks(3)); y(ks(3)); z(ks(3))];
                     el2=el2+1;
      % Calcul de l'aire du Triangle
     AireTr= 1/2*norm(cross((XB-XA), (XC-XA)));
      % Coefficients de contribution
    %face_mat(n)
      
               %[ idm abs_prop(idm,5)]
          for nmat=1:max(size(mat_Toct100Hz_5k))
                idm= find((abs_prop(:,1)-  face_mat)==0);% indice matériau
                h=c0/4*abs_prop(idm,5);
              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
              [mat_Toct100Hz_5k{nmat}]= matriceflux_oct(ai,ajk, ks,mat_Toct100Hz_5k{nmat});
          end
   end
end



   
%                     h=c0/4*abs_prop(idm,6);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_125]= matriceflux_oct(ai,ajk, ks,mat_125);
%
%             h=c0/4*abs_prop(idm,7);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_160]= matriceflux_oct(ai,ajk, ks,mat_160);
%
%
%              h=c0/4*abs_prop(idm,8);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_200]= matriceflux_oct(ai,ajk, ks,mat_200);
%              
%             h=c0/4*abs_prop(idm,9);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_250]= matriceflux_oct(ai,ajk, ks,mat_250);
%              
%              
%              h=c0/4*abs_prop(idm,10);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_315]= matriceflux_oct(ai,ajk, ks,mat_315);
%
%               h=c0/4*abs_prop(idm,11);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_400]= matriceflux_oct(ai,ajk, ks,mat_400);
%              
%             h=c0/4*abs_prop(idm,12);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_500]= matriceflux_oct(ai,ajk, ks,mat_500);
%              
%              
%              h=c0/4*abs_prop(idm,13);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_630]= matriceflux_oct(ai,ajk, ks,mat_630);
%
%              h=c0/4*abs_prop(idm,14);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_800]= matriceflux_oct(ai,ajk, ks,mat_800);              
%              
%             h=c0/4*abs_prop(idm,15);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_1k]= matriceflux_oct(ai,ajk, ks,mat_1k);       
%              
%             h=c0/4*abs_prop(idm,16);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_1250]= matriceflux_oct(ai,ajk, ks,mat_1250);              
%               
%             h=c0/4*abs_prop(idm,17);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_1600]= matriceflux_oct(ai,ajk, ks,mat_1600);              
%
%              h=c0/4*abs_prop(idm,18);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_2k]= matriceflux_oct(ai,ajk, ks,mat_2k);      
%      
%             h=c0/4*abs_prop(idm,19);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_2500]= matriceflux_oct(ai,ajk, ks,mat_2500);              
%      
%               h=c0/4*abs_prop(idm,20);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_3150]= matriceflux_oct(ai,ajk, ks,mat_3150);              
%
%    
%             h=c0/4*abs_prop(idm,21);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_4k]= matriceflux_oct(ai,ajk, ks,mat_4k);              
%    
%               h=c0/4*abs_prop(idm,22);
%              ai=22./108*h*AireTr;               ajk=7/108*h*AireTr; 
%              [mat_5k]= matriceflux_oct(ai,ajk, ks,mat_5k);        
%                          

              end
%======================================================================================================================
%                     TRANSMISSION
%======================================================================================================================
             % Tau100=-c0/4*1E-2;
%=======================   Structure =========================================
%Transm.iMT=iMT;% indice des matériaux avec coeff Transmission
%Transm.tau=[M_T(indicesMT,1) 10.^(-M_T(indicesMT,2:end)/10)];% Taux de transmission 
%===========================================================================
              
              for n=1: nel2dT
                
                  ks=el2dT(n,:);  
                  XA=[x(ks(1)); y(ks(1)); z(ks(1))];    XB=[x(ks(2)); y(ks(2)); z(ks(2))];     XC=[x(ks(3)); y(ks(3)); z(ks(3))];
             idm= find((Transm.iMT-ks(4))==0)
                  Tau=+c0/4*  Transm.tau(idm,5) ;
      % Calcul de l'aire du Triangle /3
            AireTr3= 1/6*norm(cross((XB-XA), (XC-XA)));
               TauxAireTr3=Tau*AireTr3;
               id1=find(i_pts_Trsm(:,1)==ks(1)), id2=find(i_pts_Trsm(:,1)==ks(2));id3=find(i_pts_Trsm(:,1)==ks(3));
                 ks2(1)= i_pts_Trsm(id1,2);  ks2(2)= i_pts_Trsm(id2,2);     ks2(3)= i_pts_Trsm(id3,2);
                 mat_100= matrice_transmission(mat_100,ks,ks2,TauxAireTr3);
               
%               mat_100(i_pts_Trsm(id1,2),ks(1))=mat_100(i_pts_Trsm(id1,2),ks(1))+Tau100*AireTr3; 
%               mat_100(i_pts_Trsm(id2,2),ks(2))=mat_100(i_pts_Trsm(id2,2),ks(2))+Tau100*AireTr3;
%                mat_100(i_pts_Trsm(id3,2),ks(3))=mat_100(i_pts_Trsm(id3,2),ks(3))+Tau100*AireTr3;
%                mat_100(ks(1),i_pts_Trsm(id1,2))=mat_100(ks(1),i_pts_Trsm(id1,2))+Tau100*AireTr3; 
%                mat_100(ks(2),i_pts_Trsm(id2,2))=mat_100(ks(2),i_pts_Trsm(id2,2))+Tau100*AireTr3;
%                mat_100(ks(3),i_pts_Trsm(id3,2))=mat_100(ks(3),i_pts_Trsm(id3,2))+Tau100*AireTr3;
 %[ks(1) i_pts_Trsm(id1,1)  i_pts_Trsm(id1,2)]
               end

end
%======================

