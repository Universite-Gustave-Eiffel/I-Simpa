

 % Source d'energie acoustique, second membre e.d.p.
Srce_sonore=load(strcat(domaine,'_sources.txt'));

xs=Srce_sonore(1,1) ;ys=Srce_sonore(1,2) ;zs=Srce_sonore(1,3) ;% Position de la source

dist2S=(xdf-xs).^2+(ydf-ys).^2+(zdf-zs).^2; 
rayonS2=0.15^2;% Rayon de la source de puissance injectee
VolSource=-10;
while VolSource<=0
ind=1:NDOF;rayonS2=rayonS2*1.25;
ind=ind(dist2S<rayonS2);VolSource=sum(V_VC(ind));% Volume de la source
end   
VolSource
Srce_sonore(1,4:end)=1e-12*10.^(Srce_sonore(1,4:end)/10);

%for i=1:NBLOCKS
%           pts{i}