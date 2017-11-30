
function [wi_saved]= Euler_implicite(wi,mat,MATinsta,itmax,dt)
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

 
wi_saved=zeros(max(size(wi)),itmax+1 );
 
wi_saved(:,1)=wi;
 [L,U] = ilu(mat2,struct('type','ilutp','droptol',1e-6));
for it=2:itmax+1
     RHS=MATinsta*wi;%Euler Implicite 2
    %    RHS=mat3*wi;%Cranck-Nicholson 2
  % wi=mat2\RHS;
%wi= gmres(mat2,RHS,1e-8,3000,L,U,wi);
  %wi= cgs(mat2,RHS,1e-6,200);
   [wi,flag]=bicgstab (mat2,RHS,1e-6,200,L,U,wi);
  %  wmic(it+1)=wi(ipmic);
wi_saved(:,it)=wi;
end



end