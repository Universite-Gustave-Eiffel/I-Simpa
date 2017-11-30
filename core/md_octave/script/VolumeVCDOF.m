%   Calcul de vn, Volume de chaque volume de controle autour de chaque DoF
% appel de volumeTetra.m
% c----$---1---------2---------3---------4---------5---------6---------7-c

  function [vn]= VolumeVCDOF(x,y,z,nn,nbel,Tet ,Tet_Dof)
% volumes autour de chaque point
 vn=zeros(nn,1);
for n=1:nbel
    ks=Tet(n,:);     
ksDof=Tet_Dof(n,:);      
 [v]=volumeTetra(x(ks),y(ks),z(ks));
 vn(ksDof(1))=vn(ksDof(1))+v/4; vn(ksDof(2))=vn(ksDof(2))+v/4;
 vn(ksDof(3))=vn(ksDof(3))+v/4; vn(ksDof(4))=vn(ksDof(4))+v/4;
 
 end
  

                
%===========================================================

