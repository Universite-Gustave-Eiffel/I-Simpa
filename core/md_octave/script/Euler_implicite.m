
function [wi_saved]= Euler_implicite(wi,mat,MATinsta,itmax,dt)
  % wi est la solution du REGIME ETABLI  
  
 
mat2=dt*mat+ MATinsta;%Euler Implicite 1

wi_saved=zeros(max(size(wi)),itmax );


for it=1:itmax
     RHS=MATinsta*wi;%Euler Implicite 2
    %    RHS=mat3*wi;%Cranck-Nicholson 2
    wi=mat2\RHS;
  % wi= bicgstabl(mat2,RHS,1e-5,150);
  %wi= cgs(mat2,RHS,1e-6,200);
  %  wmic(it+1)=wi(ipmic);
wi_saved(:,it)=wi;
end


end