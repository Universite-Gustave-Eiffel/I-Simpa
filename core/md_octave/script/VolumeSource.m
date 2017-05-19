%   Calcul de vn, Volume de chaque volume de controle
% appel de volumeTetra.m
% c----$---1---------2---------3---------4---------5---------6---------7-c

  function [vn]= VolumeSource(x,y,z,nn,nbel,kne)
% volumes autour de chaque point
 vn=zeros(nn,1);
for n=1:nbel
    ks=kne(n,:);            
 [v]=volumeTetra(x(ks),y(ks),z(ks));
 vn(ks(1))=vn(ks(1))+v/4; vn(ks(2))=vn(ks(2))+v/4;
 vn(ks(3))=vn(ks(3))+v/4; vn(ks(4))=vn(ks(4))+v/4;
 
 end
  

                
%===========================================================

