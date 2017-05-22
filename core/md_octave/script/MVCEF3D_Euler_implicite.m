
function [wi_saved,iter,pastemporel_sauvegarde]= MVCEF3D_Euler_implicite(wi,mat,MATinsta,itmax,dt)
  % wi est la solution du REGIME ETABLI  
  
 
mat2=dt*mat+ MATinsta;%Euler Implicite 1
%mat2=dt/2*mat+ MATinsta;%Cranck-Nicholson 1
%mat3=-dt/2*mat+ MATinsta;%Cranck-Nicholson 1

%wmic=zeros(itmax,1);% Mesure temporelle en 1 Pt de Mesure
%ipmic =200;% Indice du Pt de Mesure (au hazard!!!)
%ind=1:nn;distance_recept=rayonS2*5;
%ind=ind(dist2S>distance_recept);
%ind=ind(z(ind)>1.5);ind=ind(x(ind)<8);ind=ind(y(ind)<8);ind=ind(x(ind)>-8);
%ind=ind(y(ind)>-8);
%ipmic =ind(1);
%
%Coord_Pt_de_Mesure=[x(ipmic) y(ipmic) z(ipmic)]% Position du Pt de Mesure
%wmic(1)=wi(ipmic);
tic
iter=0;
wi_saved=zeros(max(size(wi)),round(itmax/50) );
size(wi_saved)
pastemporel_sauvegarde=10;
for it=1:pastemporel_sauvegarde:itmax
  for itsauv=1:pastemporel_sauvegarde
    RHS=MATinsta*wi;%Euler Implicite 2
    %    RHS=mat3*wi;%Cranck-Nicholson 2

    wi=mat2\RHS;
  % wi= bicgstabl(mat2,RHS,1e-5,150);
  %wi= cgs(mat2,RHS,1e-6,200);
  %  wmic(it+1)=wi(ipmic);
end
iter=iter+1;
wi_saved(:,iter)=wi;
end
toc


end