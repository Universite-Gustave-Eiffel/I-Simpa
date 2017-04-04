%   Calcul du Volume de la salle
% appel de volumeTetra.m
% c----$---1---------2---------3---------4---------5---------6---------7-c

  function [vtotal]= VolumeSalle(x,y,z,nbel,kne)

 vtotal=0;
for n=1:nbel
    ks=kne(n,:);            

 [v]=volumeTetra(x(ks),y(ks),z(ks));
 vtotal=vtotal+v;
 end
  

                
%===========================================================

