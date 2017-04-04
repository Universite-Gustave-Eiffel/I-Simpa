% *   Calcul de la somme des surfaces des parois
% c----$---1---------2---------3---------4---------5---------6---------7-c
 function [SurfSalle]= SurfacesSalle(x,y,z,nd,kne)

SurfSalle=0;
for n=1:nd
              
              ks=kne(n,:);
              XA=[x(ks(1)); y(ks(1)); z(ks(1))];
              XB=[x(ks(2)); y(ks(2)); z(ks(2))];
              XC=[x(ks(3)); y(ks(3)); z(ks(3))];
        % Calcul de l'aire du Triangle
            AireTr= 1/2*norm(cross((XB-XA),(XC-XA)));
            SurfSalle=AireTr+SurfSalle;
end
            
end
%======================

